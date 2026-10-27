// DemoRcu：一写多读内存表的 B2 形态（行地址稳定 + 行内版本号）
// 主键索引用原子槽位无锁查找；Update 原地写行并推进版本号，不分配、不换指针；
// 二级索引仍用一把读写锁保护结构，只有索引键变化时才重排节点。
// 只依赖标准库，不改动 Mdb 现有源码与构建。

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>
#include <mutex>
#include <optional>
#include <set>
#include <shared_mutex>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

namespace demo
{
    using Clock = std::chrono::steady_clock;

    struct TradingDayRow
    {
        std::int32_t Pk;
        char ExchangeId[16];
        char CurrTradingDay[16];
        char PreTradingDay[16];
    };

    class VersionedRow
    {
    public:
        static VersionedRow* Create(const TradingDayRow& initialRecord)
        {
            VersionedRow* row = new VersionedRow{};
            row->record_ = initialRecord;
            return row;
        }

        void Deallocate()
        {
            delete this;
        }

        // 只在写者侧或持有索引写锁时使用；无锁读请用 Read()
        const TradingDayRow& Record() const
        {
            return record_;
        }

        TradingDayRow Read() const
        {
            for (;;)
            {
                const std::uint64_t before = version_.load(std::memory_order_acquire);
                if ((before & 1) == 0)
                {
                    TradingDayRow copy = record_;
                    std::atomic_thread_fence(std::memory_order_acquire);
                    if (version_.load(std::memory_order_relaxed) == before)
                    {
                        return copy;
                    }
                }
                std::this_thread::yield();
            }
        }

        void Write(const TradingDayRow& newRecord)
        {
            const std::uint64_t before = version_.load(std::memory_order_relaxed);
            version_.store(before + 1, std::memory_order_relaxed);
            std::atomic_thread_fence(std::memory_order_release);
            // Pk 是主键且不可变，不重写；避免与无锁查找读 Pk 竞争
            std::memcpy(record_.ExchangeId, newRecord.ExchangeId, sizeof(record_.ExchangeId));
            std::memcpy(record_.CurrTradingDay, newRecord.CurrTradingDay, sizeof(record_.CurrTradingDay));
            std::memcpy(record_.PreTradingDay, newRecord.PreTradingDay, sizeof(record_.PreTradingDay));
            version_.store(before + 2, std::memory_order_release);
        }

    private:
        std::atomic<std::uint64_t> version_{ 0 };
        TradingDayRow record_{};
    };

    class ReaderEpoch
    {
    public:
        static constexpr std::size_t MaxReaders = 64;

        ReaderEpoch() : slots_(MaxReaders) {}

        class Guard
        {
        public:
            Guard(ReaderEpoch& owner, std::size_t readerIndex) : slot_(&owner.slots_[readerIndex])
            {
                for (;;)
                {
                    const std::uint64_t epoch = owner.currentEpoch_.load(std::memory_order_seq_cst);
                    slot_->store(epoch, std::memory_order_seq_cst);
                    if (owner.currentEpoch_.load(std::memory_order_seq_cst) == epoch)
                    {
                        return;
                    }
                    slot_->store(0, std::memory_order_seq_cst);
                }
            }

            Guard(Guard&& other) noexcept : slot_(other.slot_)
            {
                other.slot_ = nullptr;
            }

            Guard(const Guard&) = delete;
            Guard& operator=(const Guard&) = delete;
            Guard& operator=(Guard&&) = delete;

            ~Guard()
            {
                if (slot_ != nullptr)
                {
                    slot_->store(0, std::memory_order_release);
                }
            }

        private:
            std::atomic<std::uint64_t>* slot_;
        };

        void Retire(void* object, void (*reclaimer)(void*))
        {
            retired_.push_back(RetiredItem{ object, reclaimer, currentEpoch_.load(std::memory_order_seq_cst) });
        }

        void AdvanceAndReclaim()
        {
            currentEpoch_.fetch_add(1, std::memory_order_seq_cst);
            const std::uint64_t minimumPinned = MinimumPinnedEpoch();
            for (auto it = retired_.begin(); it != retired_.end();)
            {
                if (it->retiredAtEpoch < minimumPinned)
                {
                    it->reclaimer(it->object);
                    it = retired_.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        void ReclaimAllWithoutReaders()
        {
            for (const RetiredItem& item : retired_)
            {
                item.reclaimer(item.object);
            }
            retired_.clear();
        }

        std::size_t PendingRetireCount() const
        {
            return retired_.size();
        }

    private:
        struct RetiredItem
        {
            void* object;
            void (*reclaimer)(void*);
            std::uint64_t retiredAtEpoch;
        };

        std::uint64_t MinimumPinnedEpoch() const
        {
            std::uint64_t minimum = currentEpoch_.load(std::memory_order_seq_cst);
            for (const std::atomic<std::uint64_t>& slot : slots_)
            {
                const std::uint64_t epoch = slot.load(std::memory_order_seq_cst);
                if (epoch != 0 && epoch < minimum)
                {
                    minimum = epoch;
                }
            }
            return minimum;
        }

        std::atomic<std::uint64_t> currentEpoch_{ 1 };
        std::vector<std::atomic<std::uint64_t>> slots_;
        std::vector<RetiredItem> retired_;
    };

    inline std::size_t AcquireReaderIndex()
    {
        static std::atomic<std::size_t> nextIndex{ 0 };
        const std::size_t index = nextIndex.fetch_add(1, std::memory_order_relaxed);
        if (index >= ReaderEpoch::MaxReaders)
        {
            std::fprintf(stderr, "DemoRcu: reader threads exceed MaxReaders\n");
            std::abort();
        }
        return index;
    }

    inline std::size_t CurrentReaderIndex()
    {
        thread_local const std::size_t readerIndex = AcquireReaderIndex();
        return readerIndex;
    }

    class StableRowTradingDayTable
    {
    public:
        class PinnedRow
        {
        public:
            PinnedRow(ReaderEpoch::Guard guard, const VersionedRow* row) : guard_(std::move(guard)), row_(row) {}

            PinnedRow(PinnedRow&&) noexcept = default;
            PinnedRow(const PinnedRow&) = delete;
            PinnedRow& operator=(const PinnedRow&) = delete;
            PinnedRow& operator=(PinnedRow&&) = delete;

            const VersionedRow* Get() const
            {
                return row_;
            }

            TradingDayRow Read() const
            {
                return row_->Read();
            }

        private:
            ReaderEpoch::Guard guard_;
            const VersionedRow* row_;
        };

        StableRowTradingDayTable() : root_(CreateRoot(InitialCapacity)) {}

        ~StableRowTradingDayTable()
        {
            Root* root = root_.load(std::memory_order_relaxed);
            for (std::size_t position = 0; position < root->capacity; ++position)
            {
                VersionedRow* row = root->slots[position].row.load(std::memory_order_relaxed);
                if (row != nullptr && row != Tombstone())
                {
                    row->Deallocate();
                }
            }
            delete[] root->slots;
            delete root;
            epoch_.ReclaimAllWithoutReaders();
        }

        StableRowTradingDayTable(const StableRowTradingDayTable&) = delete;
        StableRowTradingDayTable& operator=(const StableRowTradingDayTable&) = delete;

        std::optional<TradingDayRow> Select(std::int32_t pk)
        {
            ReaderEpoch::Guard guard(epoch_, CurrentReaderIndex());
            const VersionedRow* row = Find(pk);
            return row == nullptr ? std::nullopt : std::optional<TradingDayRow>(row->Read());
        }

        std::vector<TradingDayRow> SelectByExchangeId(const char* exchangeId)
        {
            std::vector<TradingDayRow> copies;
            const std::string_view key(exchangeId);
            std::shared_lock guard(indexMutex_);
            const auto range = index_.equal_range(key);
            for (auto it = range.first; it != range.second; ++it)
            {
                copies.push_back((*it)->Read());
            }
            return copies;
        }

        std::optional<PinnedRow> Pin(std::int32_t pk)
        {
            ReaderEpoch::Guard guard(epoch_, CurrentReaderIndex());
            const VersionedRow* row = Find(pk);
            if (row == nullptr)
            {
                return std::nullopt;
            }
            return PinnedRow(std::move(guard), row);
        }

        bool Insert(const TradingDayRow& record)
        {
            if (Find(record.Pk) != nullptr)
            {
                return false;
            }

            RebuildForInsertIfNeeded();
            VersionedRow* row = VersionedRow::Create(record);

            Root* root = root_.load(std::memory_order_relaxed);
            std::size_t position = Hash(record.Pk) % root->capacity;
            std::size_t firstTombstone = root->capacity;
            for (std::size_t step = 0; step < root->capacity; ++step)
            {
                VersionedRow* current = root->slots[position].row.load(std::memory_order_relaxed);
                if (current == nullptr)
                {
                    const std::size_t target = firstTombstone == root->capacity ? position : firstTombstone;
                    root->slots[target].row.store(row, std::memory_order_release);
                    ++rowCount_;
                    if (firstTombstone == root->capacity)
                    {
                        ++usedSlots_;
                    }
                    std::lock_guard indexGuard(indexMutex_);
                    index_.insert(row);
                    return true;
                }
                if (current == Tombstone() && firstTombstone == root->capacity)
                {
                    firstTombstone = position;
                }
                position = (position + 1) % root->capacity;
            }

            row->Deallocate();
            return false;
        }

        bool Update(const TradingDayRow& newRecord)
        {
            VersionedRow* row = Find(newRecord.Pk);
            if (row == nullptr)
            {
                return false;
            }

            std::lock_guard guard(indexMutex_);
            const bool exchangeIdChanged = std::strcmp(row->Record().ExchangeId, newRecord.ExchangeId) != 0;
            if (exchangeIdChanged)
            {
                index_.erase(index_.find(row));
            }
            row->Write(newRecord);
            if (exchangeIdChanged)
            {
                index_.insert(row);
            }
            return true;
        }

        bool Erase(std::int32_t pk)
        {
            VersionedRow* row = Find(pk);
            if (row == nullptr)
            {
                return false;
            }

            EraseFromRoot(pk);
            {
                std::lock_guard guard(indexMutex_);
                index_.erase(index_.find(row));
            }
            --rowCount_;
            epoch_.Retire(row, ReclaimRow);
            epoch_.AdvanceAndReclaim();
            return true;
        }

        template <typename Visitor>
        void ForEach(Visitor&& visitor)
        {
            ReaderEpoch::Guard guard(epoch_, CurrentReaderIndex());
            const Root* root = root_.load(std::memory_order_acquire);
            for (std::size_t position = 0; position < root->capacity; ++position)
            {
                VersionedRow* row = root->slots[position].row.load(std::memory_order_acquire);
                if (row == nullptr || row == Tombstone())
                {
                    continue;
                }
                if (!visitor(row->Read()))
                {
                    break;
                }
            }
        }

        void CollectRetired()
        {
            epoch_.AdvanceAndReclaim();
        }

        std::size_t PendingRetireCount() const
        {
            return epoch_.PendingRetireCount();
        }

        std::size_t RowCount() const
        {
            return rowCount_;
        }

        // 仅单线程/写者侧观察用
        const VersionedRow* PeekLive(std::int32_t pk) const
        {
            return Find(pk);
        }

    private:
        struct ExchangeIdLess
        {
            using is_transparent = void;

            bool operator()(const VersionedRow* left, const VersionedRow* right) const
            {
                const int order = std::strcmp(left->Record().ExchangeId, right->Record().ExchangeId);
                return order != 0 ? order < 0 : std::less<const VersionedRow*>{}(left, right);
            }

            bool operator()(const VersionedRow* left, std::string_view exchangeId) const
            {
                return std::string_view(left->Record().ExchangeId).compare(exchangeId) < 0;
            }

            bool operator()(std::string_view exchangeId, const VersionedRow* right) const
            {
                return exchangeId.compare(std::string_view(right->Record().ExchangeId)) < 0;
            }
        };

        static constexpr std::size_t InitialCapacity = 1024;

        struct Slot
        {
            std::atomic<VersionedRow*> row{ nullptr };
        };

        struct Root
        {
            Slot* slots;
            std::size_t capacity;
        };

        static VersionedRow* Tombstone()
        {
            return reinterpret_cast<VersionedRow*>(static_cast<std::uintptr_t>(1));
        }

        static std::size_t Hash(std::int32_t pk)
        {
            return static_cast<std::size_t>(static_cast<std::uint32_t>(pk)) * 2654435761u;
        }

        static Root* CreateRoot(std::size_t capacity)
        {
            return new Root{ new Slot[capacity], capacity };
        }

        static void ReclaimRow(void* object)
        {
            static_cast<VersionedRow*>(object)->Deallocate();
        }

        static void ReclaimRoot(void* object)
        {
            Root* root = static_cast<Root*>(object);
            delete[] root->slots;
            delete root;
        }

        VersionedRow* Find(std::int32_t pk) const
        {
            const Root* root = root_.load(std::memory_order_acquire);
            std::size_t position = Hash(pk) % root->capacity;
            for (std::size_t step = 0; step < root->capacity; ++step)
            {
                VersionedRow* row = root->slots[position].row.load(std::memory_order_acquire);
                if (row == nullptr)
                {
                    return nullptr;
                }
                if (row != Tombstone() && row->Record().Pk == pk)
                {
                    return row;
                }
                position = (position + 1) % root->capacity;
            }
            return nullptr;
        }

        void EraseFromRoot(std::int32_t pk)
        {
            Root* root = root_.load(std::memory_order_relaxed);
            std::size_t position = Hash(pk) % root->capacity;
            for (std::size_t step = 0; step < root->capacity; ++step)
            {
                VersionedRow* row = root->slots[position].row.load(std::memory_order_relaxed);
                if (row != nullptr && row != Tombstone() && row->Record().Pk == pk)
                {
                    root->slots[position].row.store(Tombstone(), std::memory_order_release);
                    return;
                }
                position = (position + 1) % root->capacity;
            }
        }

        void RebuildForInsertIfNeeded()
        {
            Root* root = root_.load(std::memory_order_relaxed);
            if ((usedSlots_ + 1) * 4 <= root->capacity * 3)
            {
                return;
            }

            Root* newRoot = CreateRoot(root->capacity * 2);
            for (std::size_t position = 0; position < root->capacity; ++position)
            {
                VersionedRow* row = root->slots[position].row.load(std::memory_order_relaxed);
                if (row == nullptr || row == Tombstone())
                {
                    continue;
                }
                InsertIntoRoot(*newRoot, row);
            }
            root_.store(newRoot, std::memory_order_release);
            epoch_.Retire(root, ReclaimRoot);
            usedSlots_ = rowCount_;
        }

        static void InsertIntoRoot(Root& root, VersionedRow* row)
        {
            std::size_t position = Hash(row->Record().Pk) % root.capacity;
            while (root.slots[position].row.load(std::memory_order_relaxed) != nullptr)
            {
                position = (position + 1) % root.capacity;
            }
            root.slots[position].row.store(row, std::memory_order_relaxed);
        }

        ReaderEpoch epoch_;
        std::atomic<Root*> root_;
        std::size_t rowCount_ = 0;
        std::size_t usedSlots_ = 0;
        std::shared_mutex indexMutex_;
        std::multiset<VersionedRow*, ExchangeIdLess> index_;
    };
}

namespace
{
    class TestReport
    {
    public:
        void Check(bool condition, const char* name)
        {
            std::printf("%s=%d\n", name, condition ? 1 : 0);
            if (!condition)
            {
                ++failureCount_;
            }
        }

        int FailureCount() const
        {
            return failureCount_;
        }

    private:
        int failureCount_ = 0;
    };

    void FillRow(demo::TradingDayRow& row, std::int32_t pk, const char* exchangeId, int value)
    {
        row.Pk = pk;
        std::snprintf(row.ExchangeId, sizeof(row.ExchangeId), "%s", exchangeId);
        std::snprintf(row.CurrTradingDay, sizeof(row.CurrTradingDay), "%015d", value);
        std::snprintf(row.PreTradingDay, sizeof(row.PreTradingDay), "%015d", value);
    }

    bool PairedValueMatches(const demo::TradingDayRow& row)
    {
        return std::strcmp(row.CurrTradingDay, row.PreTradingDay) == 0;
    }

    void TestInsertAndSelect(TestReport& report)
    {
        demo::StableRowTradingDayTable table;
        demo::TradingDayRow row{};
        FillRow(row, 1, "SHFE", 100);

        report.Check(table.Insert(row), "insertAccepted");
        report.Check(table.RowCount() == 1, "insertRowCount");

        auto copy = table.Select(1);
        report.Check(copy.has_value(), "selectFound");
        report.Check(!table.Select(2).has_value(), "selectMissing");
        report.Check(copy.has_value() && std::strcmp(copy->ExchangeId, "SHFE") == 0, "selectExchangeId");

        copy->CurrTradingDay[0] = '9';
        auto again = table.Select(1);
        report.Check(again.has_value() && std::strcmp(again->CurrTradingDay, copy->CurrTradingDay) != 0, "selectReturnsDetachedCopy");

        report.Check(!table.Insert(row), "duplicateInsertRejected");
        report.Check(table.RowCount() == 1, "duplicateRowCount");

        auto byExchange = table.SelectByExchangeId("SHFE");
        report.Check(byExchange.size() == 1, "indexFindsInsertedRow");
        byExchange[0].CurrTradingDay[0] = '9';
        auto reRead = table.SelectByExchangeId("SHFE");
        report.Check(reRead.size() == 1 && std::strcmp(reRead[0].CurrTradingDay, byExchange[0].CurrTradingDay) != 0,
            "indexReturnsDetachedCopy");
    }

    void TestUpdateKeepsRowAddress(TestReport& report)
    {
        demo::StableRowTradingDayTable table;
        demo::TradingDayRow row{};
        FillRow(row, 1, "SHFE", 100);
        report.Check(table.Insert(row), "addressInsertBase");
        const demo::VersionedRow* original = table.PeekLive(1);

        demo::TradingDayRow next{};
        FillRow(next, 1, "SHFE", 200);
        report.Check(table.Update(next), "addressUpdateAccepted");
        report.Check(table.PeekLive(1) == original, "updateKeptRowAddress");
        report.Check(table.PendingRetireCount() == 0, "updateRetiredNothing");

        auto current = table.Select(1);
        report.Check(current.has_value() && std::atoi(current->CurrTradingDay) == 200, "updatedValueVisible");
        auto byExchange = table.SelectByExchangeId("SHFE");
        report.Check(byExchange.size() == 1 && std::atoi(byExchange[0].CurrTradingDay) == 200, "indexSeesUpdatedValue");

        demo::TradingDayRow missing{};
        FillRow(missing, 2, "SHFE", 300);
        report.Check(!table.Update(missing), "updateMissingRejected");
    }

    void TestUpdateMovesIndexEntry(TestReport& report)
    {
        demo::StableRowTradingDayTable table;
        demo::TradingDayRow first{};
        FillRow(first, 1, "SHFE", 100);
        demo::TradingDayRow second{};
        FillRow(second, 2, "SHFE", 200);
        report.Check(table.Insert(first) && table.Insert(second), "indexMoveInsertBase");
        const demo::VersionedRow* original = table.PeekLive(1);

        demo::TradingDayRow next{};
        FillRow(next, 1, "DCE", 300);
        report.Check(table.Update(next), "indexMoveUpdateAccepted");
        report.Check(table.PeekLive(1) == original, "indexMoveKeptRowAddress");
        report.Check(table.PendingRetireCount() == 0, "indexMoveRetiredNothing");

        auto oldKeyRows = table.SelectByExchangeId("SHFE");
        report.Check(oldKeyRows.size() == 1 && oldKeyRows[0].Pk == 2, "oldIndexKeyDropped");
        auto newKeyRows = table.SelectByExchangeId("DCE");
        report.Check(newKeyRows.size() == 1 && newKeyRows[0].Pk == 1 && std::atoi(newKeyRows[0].CurrTradingDay) == 300,
            "newIndexKeyCarriesRow");
    }

    void TestErase(TestReport& report)
    {
        demo::StableRowTradingDayTable table;
        demo::TradingDayRow row{};
        FillRow(row, 7, "SHFE", 700);
        report.Check(table.Insert(row), "eraseInsertBase");
        report.Check(table.Erase(7), "eraseAccepted");
        report.Check(!table.Select(7).has_value(), "eraseRemovedRow");
        report.Check(table.RowCount() == 0, "eraseRowCount");
        report.Check(!table.Erase(7), "eraseMissingRejected");
        report.Check(table.SelectByExchangeId("SHFE").empty(), "eraseRemovedFromIndex");

        table.CollectRetired();
        report.Check(table.PendingRetireCount() == 0, "eraseRetireCollected");
    }

    void TestPinnedRowSurvivesErase(TestReport& report)
    {
        demo::StableRowTradingDayTable table;
        demo::TradingDayRow row{};
        FillRow(row, 1, "SHFE", 100);
        report.Check(table.Insert(row), "pinnedInsertBase");

        auto pinned = table.Pin(1);
        report.Check(pinned.has_value(), "pinnedRowAvailable");
        report.Check(table.Erase(1), "pinnedEraseAccepted");
        report.Check(table.PendingRetireCount() > 0, "eraseRetireHeldWhilePinned");
        report.Check(pinned.has_value() && std::atoi(pinned->Read().CurrTradingDay) == 100, "pinnedRowStillReadable");

        pinned.reset();
        table.CollectRetired();
        report.Check(table.PendingRetireCount() == 0, "pinnedRetireCollectedAfterRelease");
    }

    void TestForEach(TestReport& report)
    {
        demo::StableRowTradingDayTable table;
        bool allInserted = true;
        for (std::int32_t pk = 1; pk <= 3; ++pk)
        {
            demo::TradingDayRow row{};
            FillRow(row, pk, pk % 2 == 0 ? "SHFE" : "DCE", static_cast<int>(pk) * 10);
            allInserted = table.Insert(row) && allInserted;
        }
        report.Check(allInserted, "forEachInsert");

        int visited = 0;
        table.ForEach([&visited](const demo::TradingDayRow& row)
        {
            if (PairedValueMatches(row))
            {
                ++visited;
            }
            return true;
        });
        report.Check(visited == 3, "forEachVisitedAllRows");
    }

    void TestRebuildForInsert(TestReport& report)
    {
        demo::StableRowTradingDayTable table;
        constexpr int RowTotal = 900;
        bool allInserted = true;
        for (int pk = 1; pk <= RowTotal; ++pk)
        {
            demo::TradingDayRow row{};
            FillRow(row, pk, pk % 2 == 0 ? "SHFE" : "DCE", pk);
            allInserted = table.Insert(row) && allInserted;
        }

        report.Check(allInserted, "rebuildAllInserted");
        report.Check(table.RowCount() == RowTotal, "rebuildRowCount");
        report.Check(table.Select(1).has_value() && table.Select(RowTotal).has_value(), "rebuildLookupAfterGrow");
        report.Check(table.SelectByExchangeId("SHFE").size() == RowTotal / 2, "rebuildIndexCount");
    }

    void TestConcurrentReadersSeeConsistentRows(TestReport& report)
    {
        demo::StableRowTradingDayTable table;
        demo::TradingDayRow base{};
        FillRow(base, 1, "SHFE", 0);
        report.Check(table.Insert(base), "concurrentInsertBase");

        constexpr int UpdateCount = 20000;
        constexpr int PrimaryReaderCount = 4;
        constexpr int IndexReaderCount = 2;
        constexpr int ReaderCount = PrimaryReaderCount + IndexReaderCount;

        std::atomic<bool> writerDone{ false };
        std::atomic<long long> tornReads{ 0 };
        std::atomic<long long> primaryReads{ 0 };
        std::atomic<long long> indexReads{ 0 };
        std::atomic<int> readyReaders{ 0 };

        std::vector<std::thread> readers;
        for (int index = 0; index < PrimaryReaderCount; ++index)
        {
            readers.emplace_back([&table, &writerDone, &tornReads, &primaryReads, &readyReaders]()
            {
                long long readCount = 0;
                long long torn = 0;
                readyReaders.fetch_add(1, std::memory_order_release);
                for (;;)
                {
                    auto copy = table.Select(1);
                    ++readCount;
                    if (!copy.has_value() || !PairedValueMatches(*copy))
                    {
                        ++torn;
                    }
                    if (writerDone.load(std::memory_order_acquire))
                    {
                        break;
                    }
                }
                primaryReads.fetch_add(readCount, std::memory_order_relaxed);
                tornReads.fetch_add(torn, std::memory_order_relaxed);
            });
        }

        for (int index = 0; index < IndexReaderCount; ++index)
        {
            readers.emplace_back([&table, &writerDone, &tornReads, &indexReads, &readyReaders]()
            {
                long long queryCount = 0;
                long long torn = 0;
                int alternate = 0;
                readyReaders.fetch_add(1, std::memory_order_release);
                for (;;)
                {
                    const char* exchangeId = (alternate++ % 2 == 0) ? "SHFE" : "DCE";
                    const auto rows = table.SelectByExchangeId(exchangeId);
                    ++queryCount;
                    for (const demo::TradingDayRow& row : rows)
                    {
                        if (!PairedValueMatches(row) || std::strcmp(row.ExchangeId, exchangeId) != 0)
                        {
                            ++torn;
                        }
                    }
                    if (writerDone.load(std::memory_order_acquire))
                    {
                        break;
                    }
                    std::this_thread::yield();
                }
                indexReads.fetch_add(queryCount, std::memory_order_relaxed);
                tornReads.fetch_add(torn, std::memory_order_relaxed);
            });
        }

        while (readyReaders.load(std::memory_order_acquire) < ReaderCount)
        {
            std::this_thread::yield();
        }

        const demo::Clock::time_point start = demo::Clock::now();
        bool allUpdatesAccepted = true;
        for (int value = 1; value <= UpdateCount; ++value)
        {
            demo::TradingDayRow next{};
            FillRow(next, 1, value % 2 == 0 ? "SHFE" : "DCE", value);
            if (!table.Update(next))
            {
                allUpdatesAccepted = false;
                break;
            }
        }
        writerDone.store(true, std::memory_order_release);

        for (std::thread& reader : readers)
        {
            reader.join();
        }
        const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(demo::Clock::now() - start).count();

        report.Check(allUpdatesAccepted, "concurrentAllUpdatesAccepted");
        report.Check(tornReads.load() == 0, "concurrentNoTornReads");
        report.Check(primaryReads.load() > 0, "concurrentPrimaryReadersMadeProgress");
        report.Check(indexReads.load() > 0, "concurrentIndexReadersMadeProgress");
        report.Check(table.RowCount() == 1, "concurrentSingleLiveRow");
        report.Check(table.PendingRetireCount() == 0, "concurrentUpdatesRetiredNothing");
        std::printf("primaryReads=%lld indexReads=%lld torn=%lld updates=%d elapsedMs=%lld\n",
            primaryReads.load(), indexReads.load(), tornReads.load(), UpdateCount, static_cast<long long>(elapsedMs));
    }
}

int main()
{
    TestReport report;
    TestInsertAndSelect(report);
    TestUpdateKeepsRowAddress(report);
    TestUpdateMovesIndexEntry(report);
    TestErase(report);
    TestPinnedRowSurvivesErase(report);
    TestForEach(report);
    TestRebuildForInsert(report);
    TestConcurrentReadersSeeConsistentRows(report);

    std::printf("failures=%d\n", report.FailureCount());
    return report.FailureCount() == 0 ? 0 : 1;
}
