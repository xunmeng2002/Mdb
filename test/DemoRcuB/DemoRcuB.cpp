// DemoRcuB：一写多读内存表的最小样例（RCU 风格，B 形态）
// 读者登记代数后无锁探测原子槽位，拷完整行即离开；
// 写者构造新行、原子换指针，旧行进退休队列，等读者离开旧代数后回收。
// 只有主键、没有二级索引：无索引场景可直接照此实现；带索引的 B2 形态见 test/DemoRcu。
// 只依赖标准库，不改动 Mdb 现有源码与构建。

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <optional>
#include <thread>
#include <utility>
#include <vector>

namespace demo
{
    using Clock = std::chrono::steady_clock;

    struct TradingDayRow
    {
        std::int32_t Pk;
        char CurrTradingDay[16];
        char PreTradingDay[16];

        static TradingDayRow* Allocate()
        {
            return new TradingDayRow{};
        }

        void Deallocate()
        {
            delete this;
        }
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
            std::fprintf(stderr, "DemoRcuB: reader threads exceed MaxReaders\n");
            std::abort();
        }
        return index;
    }

    inline std::size_t CurrentReaderIndex()
    {
        thread_local const std::size_t readerIndex = AcquireReaderIndex();
        return readerIndex;
    }

    class AtomicTradingDayTable
    {
    public:
        AtomicTradingDayTable() : root_(CreateRoot(InitialCapacity)) {}

        ~AtomicTradingDayTable()
        {
            Root* root = root_.load(std::memory_order_relaxed);
            for (std::size_t position = 0; position < root->capacity; ++position)
            {
                TradingDayRow* record = root->slots[position].record.load(std::memory_order_relaxed);
                if (record != nullptr && record != Tombstone())
                {
                    record->Deallocate();
                }
            }
            delete[] root->slots;
            delete root;
            epoch_.ReclaimAllWithoutReaders();
        }

        AtomicTradingDayTable(const AtomicTradingDayTable&) = delete;
        AtomicTradingDayTable& operator=(const AtomicTradingDayTable&) = delete;

        std::optional<TradingDayRow> Select(std::int32_t pk)
        {
            ReaderEpoch::Guard guard(epoch_, CurrentReaderIndex());
            const TradingDayRow* liveRecord = Find(pk);
            return liveRecord == nullptr ? std::nullopt : std::optional<TradingDayRow>(*liveRecord);
        }

        bool Insert(TradingDayRow* record)
        {
            RebuildForInsertIfNeeded();
            Root* root = root_.load(std::memory_order_relaxed);
            std::size_t position = Hash(record->Pk) % root->capacity;
            std::size_t firstTombstone = root->capacity;
            for (std::size_t step = 0; step < root->capacity; ++step)
            {
                TradingDayRow* current = root->slots[position].record.load(std::memory_order_relaxed);
                if (current == nullptr)
                {
                    const std::size_t target = firstTombstone == root->capacity ? position : firstTombstone;
                    root->slots[target].record.store(record, std::memory_order_release);
                    ++rowCount_;
                    if (firstTombstone == root->capacity)
                    {
                        ++usedSlots_;
                    }
                    return true;
                }
                if (current != Tombstone() && current->Pk == record->Pk)
                {
                    record->Deallocate();
                    return false;
                }
                if (current == Tombstone() && firstTombstone == root->capacity)
                {
                    firstTombstone = position;
                }
                position = (position + 1) % root->capacity;
            }
            record->Deallocate();
            return false;
        }

        bool Update(const TradingDayRow& newRecord)
        {
            TradingDayRow* replacement = TradingDayRow::Allocate();
            *replacement = newRecord;

            Root* root = root_.load(std::memory_order_relaxed);
            std::size_t position = Hash(replacement->Pk) % root->capacity;
            for (std::size_t step = 0; step < root->capacity; ++step)
            {
                TradingDayRow* current = root->slots[position].record.load(std::memory_order_relaxed);
                if (current == nullptr)
                {
                    break;
                }
                if (current != Tombstone() && current->Pk == replacement->Pk)
                {
                    root->slots[position].record.store(replacement, std::memory_order_release);
                    epoch_.Retire(current, ReclaimRow);
                    epoch_.AdvanceAndReclaim();
                    return true;
                }
                position = (position + 1) % root->capacity;
            }
            replacement->Deallocate();
            return false;
        }

        bool Erase(std::int32_t pk)
        {
            Root* root = root_.load(std::memory_order_relaxed);
            std::size_t position = Hash(pk) % root->capacity;
            for (std::size_t step = 0; step < root->capacity; ++step)
            {
                TradingDayRow* current = root->slots[position].record.load(std::memory_order_relaxed);
                if (current == nullptr)
                {
                    return false;
                }
                if (current != Tombstone() && current->Pk == pk)
                {
                    root->slots[position].record.store(Tombstone(), std::memory_order_release);
                    --rowCount_;
                    epoch_.Retire(current, ReclaimRow);
                    epoch_.AdvanceAndReclaim();
                    return true;
                }
                position = (position + 1) % root->capacity;
            }
            return false;
        }

        template <typename Visitor>
        void ForEach(Visitor&& visitor)
        {
            ReaderEpoch::Guard guard(epoch_, CurrentReaderIndex());
            const Root* root = root_.load(std::memory_order_acquire);
            for (std::size_t position = 0; position < root->capacity; ++position)
            {
                TradingDayRow* record = root->slots[position].record.load(std::memory_order_acquire);
                if (record == nullptr || record == Tombstone())
                {
                    continue;
                }
                if (!visitor(record))
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

        // 仅单线程/写者侧观察用：不登记代数，返回的指针不得跨写操作持有
        const TradingDayRow* PeekLive(std::int32_t pk) const
        {
            return Find(pk);
        }

    private:
        static constexpr std::size_t InitialCapacity = 1024;

        struct Slot
        {
            std::atomic<TradingDayRow*> record{ nullptr };
        };

        struct Root
        {
            Slot* slots;
            std::size_t capacity;
        };

        static TradingDayRow* Tombstone()
        {
            return reinterpret_cast<TradingDayRow*>(static_cast<std::uintptr_t>(1));
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
            static_cast<TradingDayRow*>(object)->Deallocate();
        }

        static void ReclaimRoot(void* object)
        {
            Root* root = static_cast<Root*>(object);
            delete[] root->slots;
            delete root;
        }

        TradingDayRow* Find(std::int32_t pk) const
        {
            const Root* root = root_.load(std::memory_order_acquire);
            std::size_t position = Hash(pk) % root->capacity;
            for (std::size_t step = 0; step < root->capacity; ++step)
            {
                TradingDayRow* record = root->slots[position].record.load(std::memory_order_acquire);
                if (record == nullptr)
                {
                    return nullptr;
                }
                if (record != Tombstone() && record->Pk == pk)
                {
                    return record;
                }
                position = (position + 1) % root->capacity;
            }
            return nullptr;
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
                TradingDayRow* record = root->slots[position].record.load(std::memory_order_relaxed);
                if (record == nullptr || record == Tombstone())
                {
                    continue;
                }
                InsertIntoRoot(*newRoot, record);
            }
            root_.store(newRoot, std::memory_order_release);
            epoch_.Retire(root, ReclaimRoot);
            usedSlots_ = rowCount_;
        }

        static void InsertIntoRoot(Root& root, TradingDayRow* record)
        {
            std::size_t position = Hash(record->Pk) % root.capacity;
            while (root.slots[position].record.load(std::memory_order_relaxed) != nullptr)
            {
                position = (position + 1) % root.capacity;
            }
            root.slots[position].record.store(record, std::memory_order_relaxed);
        }

        ReaderEpoch epoch_;
        std::atomic<Root*> root_;
        std::size_t rowCount_ = 0;
        std::size_t usedSlots_ = 0;
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

    void FillPairedValue(demo::TradingDayRow& row, std::int32_t pk, int value)
    {
        row.Pk = pk;
        std::snprintf(row.CurrTradingDay, sizeof(row.CurrTradingDay), "%015d", value);
        std::snprintf(row.PreTradingDay, sizeof(row.PreTradingDay), "%015d", value);
    }

    demo::TradingDayRow* MakeRow(std::int32_t pk, int value)
    {
        demo::TradingDayRow* row = demo::TradingDayRow::Allocate();
        FillPairedValue(*row, pk, value);
        return row;
    }

    bool PairedValueMatches(const demo::TradingDayRow& row)
    {
        return std::strcmp(row.CurrTradingDay, row.PreTradingDay) == 0;
    }

    void TestInsertAndSelect(TestReport& report)
    {
        demo::AtomicTradingDayTable table;
        report.Check(table.Insert(MakeRow(1, 100)), "insertAccepted");
        report.Check(table.RowCount() == 1, "insertRowCount");

        auto copy = table.Select(1);
        report.Check(copy.has_value(), "selectFound");
        report.Check(!table.Select(2).has_value(), "selectMissing");

        copy->CurrTradingDay[0] = '9';
        auto again = table.Select(1);
        report.Check(std::strcmp(again->CurrTradingDay, copy->CurrTradingDay) != 0, "selectReturnsDetachedCopy");

        report.Check(!table.Insert(MakeRow(1, 999)), "duplicateInsertRejected");
        report.Check(table.RowCount() == 1, "duplicateRowCount");
    }

    void TestUpdateSwapsPointerAndReclaims(TestReport& report)
    {
        demo::AtomicTradingDayTable table;
        report.Check(table.Insert(MakeRow(1, 100)), "updateInsertBase");

        const demo::TradingDayRow* previousRow = table.PeekLive(1);

        demo::TradingDayRow next{};
        FillPairedValue(next, 1, 200);
        report.Check(table.Update(next), "updateAccepted");
        report.Check(table.PeekLive(1) != previousRow, "updateSwappedPointer");
        report.Check(table.PendingRetireCount() == 0, "updateReclaimedWithNoReaders");

        auto current = table.Select(1);
        report.Check(current.has_value() && std::atoi(current->CurrTradingDay) == 200, "updatedValueVisible");

        demo::TradingDayRow missing{};
        FillPairedValue(missing, 2, 300);
        report.Check(!table.Update(missing), "updateMissingRejected");
    }

    void TestErase(TestReport& report)
    {
        demo::AtomicTradingDayTable table;
        report.Check(table.Insert(MakeRow(7, 700)), "eraseInsertBase");
        report.Check(table.Erase(7), "eraseAccepted");
        report.Check(!table.Select(7).has_value(), "eraseRemovedRow");
        report.Check(table.RowCount() == 0, "eraseRowCount");
        report.Check(!table.Erase(7), "eraseMissingRejected");

        table.CollectRetired();
        report.Check(table.PendingRetireCount() == 0, "eraseRetireCollected");
    }

    void TestForEach(TestReport& report)
    {
        demo::AtomicTradingDayTable table;
        bool allInserted = true;
        for (std::int32_t pk = 1; pk <= 3; ++pk)
        {
            allInserted = table.Insert(MakeRow(pk, static_cast<int>(pk) * 10)) && allInserted;
        }
        report.Check(allInserted, "forEachInsert");

        int visited = 0;
        table.ForEach([&visited](const demo::TradingDayRow* row)
        {
            if (PairedValueMatches(*row))
            {
                ++visited;
            }
            return true;
        });
        report.Check(visited == 3, "forEachVisitedAllRows");
    }

    void TestConcurrentReadersSeeConsistentRows(TestReport& report)
    {
        demo::AtomicTradingDayTable table;
        report.Check(table.Insert(MakeRow(1, 0)), "concurrentInsertBase");

        constexpr int UpdateCount = 20000;
        constexpr int ReaderCount = 4;
        std::atomic<bool> writerDone{ false };
        std::atomic<long long> tornReads{ 0 };
        std::atomic<long long> totalReads{ 0 };
        std::atomic<int> readyReaders{ 0 };

        std::vector<std::thread> readers;
        for (int index = 0; index < ReaderCount; ++index)
        {
            readers.emplace_back([&table, &writerDone, &tornReads, &totalReads, &readyReaders]()
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
                totalReads.fetch_add(readCount, std::memory_order_relaxed);
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
            FillPairedValue(next, 1, value);
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
        report.Check(totalReads.load() > 0, "concurrentReadersMadeProgress");
        report.Check(table.RowCount() == 1, "concurrentSingleLiveRow");
        std::printf("reads=%lld torn=%lld updates=%d elapsedMs=%lld\n",
            totalReads.load(), tornReads.load(), UpdateCount, static_cast<long long>(elapsedMs));
    }
}

int main()
{
    TestReport report;
    TestInsertAndSelect(report);
    TestUpdateSwapsPointerAndReclaims(report);
    TestErase(report);
    TestForEach(report);
    TestConcurrentReadersSeeConsistentRows(report);

    std::printf("failures=%d\n", report.FailureCount());
    return report.FailureCount() == 0 ? 0 : 1;
}
