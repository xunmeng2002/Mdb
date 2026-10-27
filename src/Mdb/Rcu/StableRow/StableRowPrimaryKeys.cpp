// 本文件由 ../Templates/Cpp/Mdb/Rcu/StableRow/StableRowPrimaryKeys.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "Rcu/StableRow/StableRowPrimaryKeys.h"
#include "Rcu/StableRow/StableRowTables.h"
#include <Spark/Core/Utility/Utility.h>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <optional>
#include <shared_mutex>
#include <thread>
#include <utility>


using namespace Spark::Core;

namespace Mdb::Rcu::StableRow
{
	std::size_t TradingDayTablePrimaryKey::HashKey(const TradingDay* record) noexcept
	{
		return TradingDayHashForTradingDayPrimaryKey()(record);
	}

	TradingDay* TradingDayTablePrimaryKey::TombstoneSlot() noexcept
	{
		return reinterpret_cast<TradingDay*>(static_cast<std::uintptr_t>(1));
	}

	bool TradingDayTablePrimaryKey::IsLiveSlot(const TradingDay* record) noexcept
	{
		return record != nullptr && record != TombstoneSlot();
	}

	TradingDayTablePrimaryKey::iterator::iterator(const TradingDaySlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept
		: slotBegin_(slotBegin), slotCount_(slotCount), position_(position)
	{
		SkipUnoccupiedSlots();
	}

	TradingDay* TradingDayTablePrimaryKey::iterator::operator*() const noexcept
	{
		return slotBegin_[position_].Record.load(std::memory_order_acquire);
	}

	TradingDayTablePrimaryKey::iterator& TradingDayTablePrimaryKey::iterator::operator++() noexcept
	{
		++position_;
		SkipUnoccupiedSlots();
		return *this;
	}

	TradingDayTablePrimaryKey::iterator TradingDayTablePrimaryKey::iterator::operator++(int) noexcept
	{
		iterator previous = *this;
		++(*this);
		return previous;
	}

	bool TradingDayTablePrimaryKey::iterator::operator==(const iterator& other) const noexcept
	{
		return slotBegin_ == other.slotBegin_ && position_ == other.position_;
	}

	bool TradingDayTablePrimaryKey::iterator::operator!=(const iterator& other) const noexcept
	{
		return !(*this == other);
	}

	void TradingDayTablePrimaryKey::iterator::SkipUnoccupiedSlots() noexcept
	{
		while (position_ < slotCount_ && !IsLiveSlot(slotBegin_[position_].Record.load(std::memory_order_relaxed)))
		{
			++position_;
		}
	}

	TradingDayTablePrimaryKey::TradingDayTablePrimaryKey(TradingDayTable* tableOwner)
		: table_(tableOwner), root_(CreateSlotArray(InitialSlotCapacity))
	{
	}

	TradingDayTablePrimaryKey::~TradingDayTablePrimaryKey()
	{
		// 表已不可达，活记录与退休对象一次性归还，不必再等读者
		ReclaimSlotArrayWithRecords(root_.load(std::memory_order_acquire));
		root_.store(nullptr, std::memory_order_relaxed);
		epoch_.ReclaimAllWithoutReaders();
	}

	std::optional<TradingDay> TradingDayTablePrimaryKey::Select(const Int32Type& Pk)
	{
		// 登记代数后按 seqlock 拷贝：写者在改同一行时版本为奇数，读者重来
		Rcu::ReaderEpoch::Guard guard(epoch_, Rcu::CurrentReaderIndex());
		TradingDay keyRecord{};
		keyRecord.Pk = Pk;
		const TradingDaySlot* slot = FindLiveSlot(&keyRecord);
		if (slot == nullptr)
		{
			return std::nullopt;
		}
		for (;;)
		{
			const std::uint64_t before = slot->Version.load(std::memory_order_acquire);
			if ((before & 1) != 0)
			{
				std::this_thread::yield();
				continue;
			}
			const TradingDay* liveRecord = slot->Record.load(std::memory_order_acquire);
			if (!IsLiveSlot(liveRecord))
			{
				return std::nullopt;
			}
			TradingDay copy{};
			std::memcpy(&copy, liveRecord, sizeof(TradingDay));
			std::atomic_thread_fence(std::memory_order_acquire);
			if (slot->Version.load(std::memory_order_relaxed) == before)
			{
				return copy;
			}
			std::this_thread::yield();
		}
	}

	TableRangeLease<TradingDayTablePrimaryKey::iterator> TradingDayTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		const iterator first(slotArray->Slots, slotArray->Capacity, 0);
		const iterator last(slotArray->Slots, slotArray->Capacity, slotArray->Capacity);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}

	bool TradingDayTablePrimaryKey::Insert(TradingDay* const record)
	{
		RebuildForInsertIfNeeded();
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindFreeSlotPosition(record);
		if (slotArray->Slots[position].Record.load(std::memory_order_relaxed) == nullptr)
		{
			++usedSlotCount_;
		}
		slotArray->Slots[position].Record.store(record, std::memory_order_release);
		++rowCount_;
		return true;
	}

	void TradingDayTablePrimaryKey::Erase(TradingDay* const record)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(record);
		assert(position < slotArray->Capacity);
		slotArray->Slots[position].Record.store(TombstoneSlot(), std::memory_order_release);
		--rowCount_;
		epoch_.Retire(record, ReclaimRecord);
		epoch_.AdvanceAndReclaim();
	}

	bool TradingDayTablePrimaryKey::CheckInsert(TradingDay* const record)
	{
		return FindLiveSlot(record) == nullptr;
	}

	TradingDay* TradingDayTablePrimaryKey::Find(const TradingDay* record)
	{
		const TradingDaySlot* slot = FindLiveSlot(record);
		return slot == nullptr ? nullptr : const_cast<TradingDay*>(slot->Record.load(std::memory_order_relaxed));
	}

	void TradingDayTablePrimaryKey::Overwrite(TradingDay* const liveRecord, const TradingDay& newRecord)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(liveRecord);
		assert(position < slotArray->Capacity);
		TradingDaySlot& slot = slotArray->Slots[position];
		const std::uint64_t before = slot.Version.load(std::memory_order_relaxed);
		slot.Version.store(before + 1, std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_release);
		// 主键字段不可变，不重写：避免与无锁查找读主键竞争
		std::memcpy(&liveRecord->CurrTradingDay, &newRecord.CurrTradingDay, sizeof(liveRecord->CurrTradingDay));
		std::memcpy(&liveRecord->PreTradingDay, &newRecord.PreTradingDay, sizeof(liveRecord->PreTradingDay));
		slot.Version.store(before + 2, std::memory_order_release);
	}

	void TradingDayTablePrimaryKey::Clear()
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		root_.store(CreateSlotArray(InitialSlotCapacity), std::memory_order_release);
		rowCount_ = 0;
		usedSlotCount_ = 0;
		epoch_.Retire(previousSlotArray, ReclaimSlotArrayWithRecords);
		epoch_.AdvanceAndReclaim();
	}

	const TradingDaySlot* TradingDayTablePrimaryKey::FindLiveSlot(const TradingDay* keyRecord) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		std::size_t position = HashKey(keyRecord) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const TradingDaySlot* slot = &slotArray->Slots[position];
			const TradingDay* candidate = slot->Record.load(std::memory_order_acquire);
			if (candidate == nullptr)
			{
				return nullptr;
			}
			if (IsLiveSlot(candidate) && TradingDayEqualForTradingDayPrimaryKey()(candidate, keyRecord))
			{
				return slot;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return nullptr;
	}

	std::size_t TradingDayTablePrimaryKey::FindOccupiedSlotPosition(const TradingDay* record) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		std::size_t position = HashKey(record) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const TradingDay* candidate = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (candidate == nullptr)
			{
				break;
			}
			if (candidate == record)
			{
				return position;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return slotArray->Capacity;
	}

	std::size_t TradingDayTablePrimaryKey::FindFreeSlotPosition(const TradingDay* keyRecord) const
	{
		return FindFreeSlotPositionIn(*root_.load(std::memory_order_relaxed), keyRecord);
	}

	std::size_t TradingDayTablePrimaryKey::FindFreeSlotPositionIn(const SlotArray& slotArray, const TradingDay* keyRecord)
	{
		std::size_t position = HashKey(keyRecord) % slotArray.Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray.Capacity; ++probedSlotCount)
		{
			if (!IsLiveSlot(slotArray.Slots[position].Record.load(std::memory_order_relaxed)))
			{
				return position;
			}
			position = (position + 1) % slotArray.Capacity;
		}
		return slotArray.Capacity;
	}

	void TradingDayTablePrimaryKey::RebuildForInsertIfNeeded()
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		if ((usedSlotCount_ + 1) * 4 <= slotArray->Capacity * 3)
		{
			return;
		}
		Rebuild(slotArray->Capacity * 2);
	}

	void TradingDayTablePrimaryKey::Rebuild(std::size_t capacity)
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		SlotArray* slotArray = CreateSlotArray(capacity);
		ForEachRecord([slotArray](TradingDay* record)
		{
			const std::size_t position = FindFreeSlotPositionIn(*slotArray, record);
			slotArray->Slots[position].Record.store(record, std::memory_order_relaxed);
			return true;
		});
		root_.store(slotArray, std::memory_order_release);
		usedSlotCount_ = rowCount_;
		// 记录已搬进新槽数组，旧数组只回收槽位本身
		epoch_.Retire(previousSlotArray, ReclaimSlotArray);
		epoch_.AdvanceAndReclaim();
	}

	TradingDayTablePrimaryKey::SlotArray* TradingDayTablePrimaryKey::CreateSlotArray(std::size_t capacity)
	{
		return new SlotArray{ new TradingDaySlot[capacity]{}, capacity };
	}

	void TradingDayTablePrimaryKey::ReclaimSlotArray(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void TradingDayTablePrimaryKey::ReclaimSlotArrayWithRecords(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		for (std::size_t position = 0; position < slotArray->Capacity; ++position)
		{
			TradingDay* record = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (IsLiveSlot(record))
			{
				record->Deallocate();
			}
		}
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void TradingDayTablePrimaryKey::ReclaimRecord(void* object) noexcept
	{
		static_cast<TradingDay*>(object)->Deallocate();
	}

	std::size_t ExchangeTablePrimaryKey::HashKey(const Exchange* record) noexcept
	{
		return ExchangeHashForExchangePrimaryKey()(record);
	}

	Exchange* ExchangeTablePrimaryKey::TombstoneSlot() noexcept
	{
		return reinterpret_cast<Exchange*>(static_cast<std::uintptr_t>(1));
	}

	bool ExchangeTablePrimaryKey::IsLiveSlot(const Exchange* record) noexcept
	{
		return record != nullptr && record != TombstoneSlot();
	}

	ExchangeTablePrimaryKey::iterator::iterator(const ExchangeSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept
		: slotBegin_(slotBegin), slotCount_(slotCount), position_(position)
	{
		SkipUnoccupiedSlots();
	}

	Exchange* ExchangeTablePrimaryKey::iterator::operator*() const noexcept
	{
		return slotBegin_[position_].Record.load(std::memory_order_acquire);
	}

	ExchangeTablePrimaryKey::iterator& ExchangeTablePrimaryKey::iterator::operator++() noexcept
	{
		++position_;
		SkipUnoccupiedSlots();
		return *this;
	}

	ExchangeTablePrimaryKey::iterator ExchangeTablePrimaryKey::iterator::operator++(int) noexcept
	{
		iterator previous = *this;
		++(*this);
		return previous;
	}

	bool ExchangeTablePrimaryKey::iterator::operator==(const iterator& other) const noexcept
	{
		return slotBegin_ == other.slotBegin_ && position_ == other.position_;
	}

	bool ExchangeTablePrimaryKey::iterator::operator!=(const iterator& other) const noexcept
	{
		return !(*this == other);
	}

	void ExchangeTablePrimaryKey::iterator::SkipUnoccupiedSlots() noexcept
	{
		while (position_ < slotCount_ && !IsLiveSlot(slotBegin_[position_].Record.load(std::memory_order_relaxed)))
		{
			++position_;
		}
	}

	ExchangeTablePrimaryKey::ExchangeTablePrimaryKey(ExchangeTable* tableOwner)
		: table_(tableOwner), root_(CreateSlotArray(InitialSlotCapacity))
	{
	}

	ExchangeTablePrimaryKey::~ExchangeTablePrimaryKey()
	{
		// 表已不可达，活记录与退休对象一次性归还，不必再等读者
		ReclaimSlotArrayWithRecords(root_.load(std::memory_order_acquire));
		root_.store(nullptr, std::memory_order_relaxed);
		epoch_.ReclaimAllWithoutReaders();
	}

	std::optional<Exchange> ExchangeTablePrimaryKey::Select(const ExchangeIdType& ExchangeId)
	{
		// 登记代数后按 seqlock 拷贝：写者在改同一行时版本为奇数，读者重来
		Rcu::ReaderEpoch::Guard guard(epoch_, Rcu::CurrentReaderIndex());
		Exchange keyRecord{};
		Utility::Strcpy(keyRecord.ExchangeId, ExchangeId);
		const ExchangeSlot* slot = FindLiveSlot(&keyRecord);
		if (slot == nullptr)
		{
			return std::nullopt;
		}
		for (;;)
		{
			const std::uint64_t before = slot->Version.load(std::memory_order_acquire);
			if ((before & 1) != 0)
			{
				std::this_thread::yield();
				continue;
			}
			const Exchange* liveRecord = slot->Record.load(std::memory_order_acquire);
			if (!IsLiveSlot(liveRecord))
			{
				return std::nullopt;
			}
			Exchange copy{};
			std::memcpy(&copy, liveRecord, sizeof(Exchange));
			std::atomic_thread_fence(std::memory_order_acquire);
			if (slot->Version.load(std::memory_order_relaxed) == before)
			{
				return copy;
			}
			std::this_thread::yield();
		}
	}

	TableRangeLease<ExchangeTablePrimaryKey::iterator> ExchangeTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		const iterator first(slotArray->Slots, slotArray->Capacity, 0);
		const iterator last(slotArray->Slots, slotArray->Capacity, slotArray->Capacity);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}

	bool ExchangeTablePrimaryKey::Insert(Exchange* const record)
	{
		RebuildForInsertIfNeeded();
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindFreeSlotPosition(record);
		if (slotArray->Slots[position].Record.load(std::memory_order_relaxed) == nullptr)
		{
			++usedSlotCount_;
		}
		slotArray->Slots[position].Record.store(record, std::memory_order_release);
		++rowCount_;
		return true;
	}

	void ExchangeTablePrimaryKey::Erase(Exchange* const record)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(record);
		assert(position < slotArray->Capacity);
		slotArray->Slots[position].Record.store(TombstoneSlot(), std::memory_order_release);
		--rowCount_;
		epoch_.Retire(record, ReclaimRecord);
		epoch_.AdvanceAndReclaim();
	}

	bool ExchangeTablePrimaryKey::CheckInsert(Exchange* const record)
	{
		return FindLiveSlot(record) == nullptr;
	}

	Exchange* ExchangeTablePrimaryKey::Find(const Exchange* record)
	{
		const ExchangeSlot* slot = FindLiveSlot(record);
		return slot == nullptr ? nullptr : const_cast<Exchange*>(slot->Record.load(std::memory_order_relaxed));
	}

	void ExchangeTablePrimaryKey::Overwrite(Exchange* const liveRecord, const Exchange& newRecord)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(liveRecord);
		assert(position < slotArray->Capacity);
		ExchangeSlot& slot = slotArray->Slots[position];
		const std::uint64_t before = slot.Version.load(std::memory_order_relaxed);
		slot.Version.store(before + 1, std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_release);
		// 主键字段不可变，不重写：避免与无锁查找读主键竞争
		std::memcpy(&liveRecord->ExchangeName, &newRecord.ExchangeName, sizeof(liveRecord->ExchangeName));
		slot.Version.store(before + 2, std::memory_order_release);
	}

	void ExchangeTablePrimaryKey::Clear()
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		root_.store(CreateSlotArray(InitialSlotCapacity), std::memory_order_release);
		rowCount_ = 0;
		usedSlotCount_ = 0;
		epoch_.Retire(previousSlotArray, ReclaimSlotArrayWithRecords);
		epoch_.AdvanceAndReclaim();
	}

	const ExchangeSlot* ExchangeTablePrimaryKey::FindLiveSlot(const Exchange* keyRecord) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		std::size_t position = HashKey(keyRecord) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const ExchangeSlot* slot = &slotArray->Slots[position];
			const Exchange* candidate = slot->Record.load(std::memory_order_acquire);
			if (candidate == nullptr)
			{
				return nullptr;
			}
			if (IsLiveSlot(candidate) && ExchangeEqualForExchangePrimaryKey()(candidate, keyRecord))
			{
				return slot;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return nullptr;
	}

	std::size_t ExchangeTablePrimaryKey::FindOccupiedSlotPosition(const Exchange* record) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		std::size_t position = HashKey(record) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const Exchange* candidate = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (candidate == nullptr)
			{
				break;
			}
			if (candidate == record)
			{
				return position;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return slotArray->Capacity;
	}

	std::size_t ExchangeTablePrimaryKey::FindFreeSlotPosition(const Exchange* keyRecord) const
	{
		return FindFreeSlotPositionIn(*root_.load(std::memory_order_relaxed), keyRecord);
	}

	std::size_t ExchangeTablePrimaryKey::FindFreeSlotPositionIn(const SlotArray& slotArray, const Exchange* keyRecord)
	{
		std::size_t position = HashKey(keyRecord) % slotArray.Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray.Capacity; ++probedSlotCount)
		{
			if (!IsLiveSlot(slotArray.Slots[position].Record.load(std::memory_order_relaxed)))
			{
				return position;
			}
			position = (position + 1) % slotArray.Capacity;
		}
		return slotArray.Capacity;
	}

	void ExchangeTablePrimaryKey::RebuildForInsertIfNeeded()
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		if ((usedSlotCount_ + 1) * 4 <= slotArray->Capacity * 3)
		{
			return;
		}
		Rebuild(slotArray->Capacity * 2);
	}

	void ExchangeTablePrimaryKey::Rebuild(std::size_t capacity)
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		SlotArray* slotArray = CreateSlotArray(capacity);
		ForEachRecord([slotArray](Exchange* record)
		{
			const std::size_t position = FindFreeSlotPositionIn(*slotArray, record);
			slotArray->Slots[position].Record.store(record, std::memory_order_relaxed);
			return true;
		});
		root_.store(slotArray, std::memory_order_release);
		usedSlotCount_ = rowCount_;
		// 记录已搬进新槽数组，旧数组只回收槽位本身
		epoch_.Retire(previousSlotArray, ReclaimSlotArray);
		epoch_.AdvanceAndReclaim();
	}

	ExchangeTablePrimaryKey::SlotArray* ExchangeTablePrimaryKey::CreateSlotArray(std::size_t capacity)
	{
		return new SlotArray{ new ExchangeSlot[capacity]{}, capacity };
	}

	void ExchangeTablePrimaryKey::ReclaimSlotArray(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void ExchangeTablePrimaryKey::ReclaimSlotArrayWithRecords(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		for (std::size_t position = 0; position < slotArray->Capacity; ++position)
		{
			Exchange* record = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (IsLiveSlot(record))
			{
				record->Deallocate();
			}
		}
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void ExchangeTablePrimaryKey::ReclaimRecord(void* object) noexcept
	{
		static_cast<Exchange*>(object)->Deallocate();
	}

	std::size_t ProductTablePrimaryKey::HashKey(const Product* record) noexcept
	{
		return ProductHashForProductPrimaryKey()(record);
	}

	Product* ProductTablePrimaryKey::TombstoneSlot() noexcept
	{
		return reinterpret_cast<Product*>(static_cast<std::uintptr_t>(1));
	}

	bool ProductTablePrimaryKey::IsLiveSlot(const Product* record) noexcept
	{
		return record != nullptr && record != TombstoneSlot();
	}

	ProductTablePrimaryKey::iterator::iterator(const ProductSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept
		: slotBegin_(slotBegin), slotCount_(slotCount), position_(position)
	{
		SkipUnoccupiedSlots();
	}

	Product* ProductTablePrimaryKey::iterator::operator*() const noexcept
	{
		return slotBegin_[position_].Record.load(std::memory_order_acquire);
	}

	ProductTablePrimaryKey::iterator& ProductTablePrimaryKey::iterator::operator++() noexcept
	{
		++position_;
		SkipUnoccupiedSlots();
		return *this;
	}

	ProductTablePrimaryKey::iterator ProductTablePrimaryKey::iterator::operator++(int) noexcept
	{
		iterator previous = *this;
		++(*this);
		return previous;
	}

	bool ProductTablePrimaryKey::iterator::operator==(const iterator& other) const noexcept
	{
		return slotBegin_ == other.slotBegin_ && position_ == other.position_;
	}

	bool ProductTablePrimaryKey::iterator::operator!=(const iterator& other) const noexcept
	{
		return !(*this == other);
	}

	void ProductTablePrimaryKey::iterator::SkipUnoccupiedSlots() noexcept
	{
		while (position_ < slotCount_ && !IsLiveSlot(slotBegin_[position_].Record.load(std::memory_order_relaxed)))
		{
			++position_;
		}
	}

	ProductTablePrimaryKey::ProductTablePrimaryKey(ProductTable* tableOwner)
		: table_(tableOwner), root_(CreateSlotArray(InitialSlotCapacity))
	{
	}

	ProductTablePrimaryKey::~ProductTablePrimaryKey()
	{
		// 表已不可达，活记录与退休对象一次性归还，不必再等读者
		ReclaimSlotArrayWithRecords(root_.load(std::memory_order_acquire));
		root_.store(nullptr, std::memory_order_relaxed);
		epoch_.ReclaimAllWithoutReaders();
	}

	std::optional<Product> ProductTablePrimaryKey::Select(const ExchangeIdType& ExchangeId, const ProductIdType& ProductId)
	{
		// 登记代数后按 seqlock 拷贝：写者在改同一行时版本为奇数，读者重来
		Rcu::ReaderEpoch::Guard guard(epoch_, Rcu::CurrentReaderIndex());
		Product keyRecord{};
		Utility::Strcpy(keyRecord.ExchangeId, ExchangeId);
		Utility::Strcpy(keyRecord.ProductId, ProductId);
		const ProductSlot* slot = FindLiveSlot(&keyRecord);
		if (slot == nullptr)
		{
			return std::nullopt;
		}
		for (;;)
		{
			const std::uint64_t before = slot->Version.load(std::memory_order_acquire);
			if ((before & 1) != 0)
			{
				std::this_thread::yield();
				continue;
			}
			const Product* liveRecord = slot->Record.load(std::memory_order_acquire);
			if (!IsLiveSlot(liveRecord))
			{
				return std::nullopt;
			}
			Product copy{};
			std::memcpy(&copy, liveRecord, sizeof(Product));
			std::atomic_thread_fence(std::memory_order_acquire);
			if (slot->Version.load(std::memory_order_relaxed) == before)
			{
				return copy;
			}
			std::this_thread::yield();
		}
	}

	TableRangeLease<ProductTablePrimaryKey::iterator> ProductTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		const iterator first(slotArray->Slots, slotArray->Capacity, 0);
		const iterator last(slotArray->Slots, slotArray->Capacity, slotArray->Capacity);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}

	bool ProductTablePrimaryKey::Insert(Product* const record)
	{
		RebuildForInsertIfNeeded();
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindFreeSlotPosition(record);
		if (slotArray->Slots[position].Record.load(std::memory_order_relaxed) == nullptr)
		{
			++usedSlotCount_;
		}
		slotArray->Slots[position].Record.store(record, std::memory_order_release);
		++rowCount_;
		return true;
	}

	void ProductTablePrimaryKey::Erase(Product* const record)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(record);
		assert(position < slotArray->Capacity);
		slotArray->Slots[position].Record.store(TombstoneSlot(), std::memory_order_release);
		--rowCount_;
		epoch_.Retire(record, ReclaimRecord);
		epoch_.AdvanceAndReclaim();
	}

	bool ProductTablePrimaryKey::CheckInsert(Product* const record)
	{
		return FindLiveSlot(record) == nullptr;
	}

	Product* ProductTablePrimaryKey::Find(const Product* record)
	{
		const ProductSlot* slot = FindLiveSlot(record);
		return slot == nullptr ? nullptr : const_cast<Product*>(slot->Record.load(std::memory_order_relaxed));
	}

	void ProductTablePrimaryKey::Overwrite(Product* const liveRecord, const Product& newRecord)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(liveRecord);
		assert(position < slotArray->Capacity);
		ProductSlot& slot = slotArray->Slots[position];
		const std::uint64_t before = slot.Version.load(std::memory_order_relaxed);
		slot.Version.store(before + 1, std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_release);
		// 主键字段不可变，不重写：避免与无锁查找读主键竞争
		std::memcpy(&liveRecord->ProductName, &newRecord.ProductName, sizeof(liveRecord->ProductName));
		std::memcpy(&liveRecord->ProductClass, &newRecord.ProductClass, sizeof(liveRecord->ProductClass));
		std::memcpy(&liveRecord->VolumeMultiple, &newRecord.VolumeMultiple, sizeof(liveRecord->VolumeMultiple));
		std::memcpy(&liveRecord->PriceTick, &newRecord.PriceTick, sizeof(liveRecord->PriceTick));
		std::memcpy(&liveRecord->MaxMarketOrderVolume, &newRecord.MaxMarketOrderVolume, sizeof(liveRecord->MaxMarketOrderVolume));
		std::memcpy(&liveRecord->MinMarketOrderVolume, &newRecord.MinMarketOrderVolume, sizeof(liveRecord->MinMarketOrderVolume));
		std::memcpy(&liveRecord->MaxLimitOrderVolume, &newRecord.MaxLimitOrderVolume, sizeof(liveRecord->MaxLimitOrderVolume));
		std::memcpy(&liveRecord->MinLimitOrderVolume, &newRecord.MinLimitOrderVolume, sizeof(liveRecord->MinLimitOrderVolume));
		std::memcpy(&liveRecord->SessionName, &newRecord.SessionName, sizeof(liveRecord->SessionName));
		slot.Version.store(before + 2, std::memory_order_release);
	}

	void ProductTablePrimaryKey::Clear()
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		root_.store(CreateSlotArray(InitialSlotCapacity), std::memory_order_release);
		rowCount_ = 0;
		usedSlotCount_ = 0;
		epoch_.Retire(previousSlotArray, ReclaimSlotArrayWithRecords);
		epoch_.AdvanceAndReclaim();
	}

	const ProductSlot* ProductTablePrimaryKey::FindLiveSlot(const Product* keyRecord) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		std::size_t position = HashKey(keyRecord) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const ProductSlot* slot = &slotArray->Slots[position];
			const Product* candidate = slot->Record.load(std::memory_order_acquire);
			if (candidate == nullptr)
			{
				return nullptr;
			}
			if (IsLiveSlot(candidate) && ProductEqualForProductPrimaryKey()(candidate, keyRecord))
			{
				return slot;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return nullptr;
	}

	std::size_t ProductTablePrimaryKey::FindOccupiedSlotPosition(const Product* record) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		std::size_t position = HashKey(record) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const Product* candidate = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (candidate == nullptr)
			{
				break;
			}
			if (candidate == record)
			{
				return position;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return slotArray->Capacity;
	}

	std::size_t ProductTablePrimaryKey::FindFreeSlotPosition(const Product* keyRecord) const
	{
		return FindFreeSlotPositionIn(*root_.load(std::memory_order_relaxed), keyRecord);
	}

	std::size_t ProductTablePrimaryKey::FindFreeSlotPositionIn(const SlotArray& slotArray, const Product* keyRecord)
	{
		std::size_t position = HashKey(keyRecord) % slotArray.Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray.Capacity; ++probedSlotCount)
		{
			if (!IsLiveSlot(slotArray.Slots[position].Record.load(std::memory_order_relaxed)))
			{
				return position;
			}
			position = (position + 1) % slotArray.Capacity;
		}
		return slotArray.Capacity;
	}

	void ProductTablePrimaryKey::RebuildForInsertIfNeeded()
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		if ((usedSlotCount_ + 1) * 4 <= slotArray->Capacity * 3)
		{
			return;
		}
		Rebuild(slotArray->Capacity * 2);
	}

	void ProductTablePrimaryKey::Rebuild(std::size_t capacity)
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		SlotArray* slotArray = CreateSlotArray(capacity);
		ForEachRecord([slotArray](Product* record)
		{
			const std::size_t position = FindFreeSlotPositionIn(*slotArray, record);
			slotArray->Slots[position].Record.store(record, std::memory_order_relaxed);
			return true;
		});
		root_.store(slotArray, std::memory_order_release);
		usedSlotCount_ = rowCount_;
		// 记录已搬进新槽数组，旧数组只回收槽位本身
		epoch_.Retire(previousSlotArray, ReclaimSlotArray);
		epoch_.AdvanceAndReclaim();
	}

	ProductTablePrimaryKey::SlotArray* ProductTablePrimaryKey::CreateSlotArray(std::size_t capacity)
	{
		return new SlotArray{ new ProductSlot[capacity]{}, capacity };
	}

	void ProductTablePrimaryKey::ReclaimSlotArray(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void ProductTablePrimaryKey::ReclaimSlotArrayWithRecords(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		for (std::size_t position = 0; position < slotArray->Capacity; ++position)
		{
			Product* record = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (IsLiveSlot(record))
			{
				record->Deallocate();
			}
		}
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void ProductTablePrimaryKey::ReclaimRecord(void* object) noexcept
	{
		static_cast<Product*>(object)->Deallocate();
	}

	std::size_t InstrumentTablePrimaryKey::HashKey(const Instrument* record) noexcept
	{
		return InstrumentHashForInstrumentPrimaryKey()(record);
	}

	Instrument* InstrumentTablePrimaryKey::TombstoneSlot() noexcept
	{
		return reinterpret_cast<Instrument*>(static_cast<std::uintptr_t>(1));
	}

	bool InstrumentTablePrimaryKey::IsLiveSlot(const Instrument* record) noexcept
	{
		return record != nullptr && record != TombstoneSlot();
	}

	InstrumentTablePrimaryKey::iterator::iterator(const InstrumentSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept
		: slotBegin_(slotBegin), slotCount_(slotCount), position_(position)
	{
		SkipUnoccupiedSlots();
	}

	Instrument* InstrumentTablePrimaryKey::iterator::operator*() const noexcept
	{
		return slotBegin_[position_].Record.load(std::memory_order_acquire);
	}

	InstrumentTablePrimaryKey::iterator& InstrumentTablePrimaryKey::iterator::operator++() noexcept
	{
		++position_;
		SkipUnoccupiedSlots();
		return *this;
	}

	InstrumentTablePrimaryKey::iterator InstrumentTablePrimaryKey::iterator::operator++(int) noexcept
	{
		iterator previous = *this;
		++(*this);
		return previous;
	}

	bool InstrumentTablePrimaryKey::iterator::operator==(const iterator& other) const noexcept
	{
		return slotBegin_ == other.slotBegin_ && position_ == other.position_;
	}

	bool InstrumentTablePrimaryKey::iterator::operator!=(const iterator& other) const noexcept
	{
		return !(*this == other);
	}

	void InstrumentTablePrimaryKey::iterator::SkipUnoccupiedSlots() noexcept
	{
		while (position_ < slotCount_ && !IsLiveSlot(slotBegin_[position_].Record.load(std::memory_order_relaxed)))
		{
			++position_;
		}
	}

	InstrumentTablePrimaryKey::InstrumentTablePrimaryKey(InstrumentTable* tableOwner)
		: table_(tableOwner), root_(CreateSlotArray(InitialSlotCapacity))
	{
	}

	InstrumentTablePrimaryKey::~InstrumentTablePrimaryKey()
	{
		// 表已不可达，活记录与退休对象一次性归还，不必再等读者
		ReclaimSlotArrayWithRecords(root_.load(std::memory_order_acquire));
		root_.store(nullptr, std::memory_order_relaxed);
		epoch_.ReclaimAllWithoutReaders();
	}

	std::optional<Instrument> InstrumentTablePrimaryKey::Select(const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId)
	{
		// 登记代数后按 seqlock 拷贝：写者在改同一行时版本为奇数，读者重来
		Rcu::ReaderEpoch::Guard guard(epoch_, Rcu::CurrentReaderIndex());
		Instrument keyRecord{};
		Utility::Strcpy(keyRecord.ExchangeId, ExchangeId);
		Utility::Strcpy(keyRecord.InstrumentId, InstrumentId);
		const InstrumentSlot* slot = FindLiveSlot(&keyRecord);
		if (slot == nullptr)
		{
			return std::nullopt;
		}
		for (;;)
		{
			const std::uint64_t before = slot->Version.load(std::memory_order_acquire);
			if ((before & 1) != 0)
			{
				std::this_thread::yield();
				continue;
			}
			const Instrument* liveRecord = slot->Record.load(std::memory_order_acquire);
			if (!IsLiveSlot(liveRecord))
			{
				return std::nullopt;
			}
			Instrument copy{};
			std::memcpy(&copy, liveRecord, sizeof(Instrument));
			std::atomic_thread_fence(std::memory_order_acquire);
			if (slot->Version.load(std::memory_order_relaxed) == before)
			{
				return copy;
			}
			std::this_thread::yield();
		}
	}

	TableRangeLease<InstrumentTablePrimaryKey::iterator> InstrumentTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		const iterator first(slotArray->Slots, slotArray->Capacity, 0);
		const iterator last(slotArray->Slots, slotArray->Capacity, slotArray->Capacity);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}

	bool InstrumentTablePrimaryKey::Insert(Instrument* const record)
	{
		RebuildForInsertIfNeeded();
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindFreeSlotPosition(record);
		if (slotArray->Slots[position].Record.load(std::memory_order_relaxed) == nullptr)
		{
			++usedSlotCount_;
		}
		slotArray->Slots[position].Record.store(record, std::memory_order_release);
		++rowCount_;
		return true;
	}

	void InstrumentTablePrimaryKey::Erase(Instrument* const record)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(record);
		assert(position < slotArray->Capacity);
		slotArray->Slots[position].Record.store(TombstoneSlot(), std::memory_order_release);
		--rowCount_;
		epoch_.Retire(record, ReclaimRecord);
		epoch_.AdvanceAndReclaim();
	}

	bool InstrumentTablePrimaryKey::CheckInsert(Instrument* const record)
	{
		return FindLiveSlot(record) == nullptr;
	}

	Instrument* InstrumentTablePrimaryKey::Find(const Instrument* record)
	{
		const InstrumentSlot* slot = FindLiveSlot(record);
		return slot == nullptr ? nullptr : const_cast<Instrument*>(slot->Record.load(std::memory_order_relaxed));
	}

	void InstrumentTablePrimaryKey::Overwrite(Instrument* const liveRecord, const Instrument& newRecord)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(liveRecord);
		assert(position < slotArray->Capacity);
		InstrumentSlot& slot = slotArray->Slots[position];
		const std::uint64_t before = slot.Version.load(std::memory_order_relaxed);
		slot.Version.store(before + 1, std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_release);
		// 主键字段不可变，不重写：避免与无锁查找读主键竞争
		std::memcpy(&liveRecord->ExchangeInstId, &newRecord.ExchangeInstId, sizeof(liveRecord->ExchangeInstId));
		std::memcpy(&liveRecord->InstrumentName, &newRecord.InstrumentName, sizeof(liveRecord->InstrumentName));
		std::memcpy(&liveRecord->ProductId, &newRecord.ProductId, sizeof(liveRecord->ProductId));
		std::memcpy(&liveRecord->ProductClass, &newRecord.ProductClass, sizeof(liveRecord->ProductClass));
		std::memcpy(&liveRecord->InstrumentClass, &newRecord.InstrumentClass, sizeof(liveRecord->InstrumentClass));
		std::memcpy(&liveRecord->Rank, &newRecord.Rank, sizeof(liveRecord->Rank));
		std::memcpy(&liveRecord->VolumeMultiple, &newRecord.VolumeMultiple, sizeof(liveRecord->VolumeMultiple));
		std::memcpy(&liveRecord->PriceTick, &newRecord.PriceTick, sizeof(liveRecord->PriceTick));
		std::memcpy(&liveRecord->MaxMarketOrderVolume, &newRecord.MaxMarketOrderVolume, sizeof(liveRecord->MaxMarketOrderVolume));
		std::memcpy(&liveRecord->MinMarketOrderVolume, &newRecord.MinMarketOrderVolume, sizeof(liveRecord->MinMarketOrderVolume));
		std::memcpy(&liveRecord->MaxLimitOrderVolume, &newRecord.MaxLimitOrderVolume, sizeof(liveRecord->MaxLimitOrderVolume));
		std::memcpy(&liveRecord->MinLimitOrderVolume, &newRecord.MinLimitOrderVolume, sizeof(liveRecord->MinLimitOrderVolume));
		std::memcpy(&liveRecord->SessionName, &newRecord.SessionName, sizeof(liveRecord->SessionName));
		slot.Version.store(before + 2, std::memory_order_release);
	}

	void InstrumentTablePrimaryKey::Clear()
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		root_.store(CreateSlotArray(InitialSlotCapacity), std::memory_order_release);
		rowCount_ = 0;
		usedSlotCount_ = 0;
		epoch_.Retire(previousSlotArray, ReclaimSlotArrayWithRecords);
		epoch_.AdvanceAndReclaim();
	}

	const InstrumentSlot* InstrumentTablePrimaryKey::FindLiveSlot(const Instrument* keyRecord) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		std::size_t position = HashKey(keyRecord) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const InstrumentSlot* slot = &slotArray->Slots[position];
			const Instrument* candidate = slot->Record.load(std::memory_order_acquire);
			if (candidate == nullptr)
			{
				return nullptr;
			}
			if (IsLiveSlot(candidate) && InstrumentEqualForInstrumentPrimaryKey()(candidate, keyRecord))
			{
				return slot;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return nullptr;
	}

	std::size_t InstrumentTablePrimaryKey::FindOccupiedSlotPosition(const Instrument* record) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		std::size_t position = HashKey(record) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const Instrument* candidate = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (candidate == nullptr)
			{
				break;
			}
			if (candidate == record)
			{
				return position;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return slotArray->Capacity;
	}

	std::size_t InstrumentTablePrimaryKey::FindFreeSlotPosition(const Instrument* keyRecord) const
	{
		return FindFreeSlotPositionIn(*root_.load(std::memory_order_relaxed), keyRecord);
	}

	std::size_t InstrumentTablePrimaryKey::FindFreeSlotPositionIn(const SlotArray& slotArray, const Instrument* keyRecord)
	{
		std::size_t position = HashKey(keyRecord) % slotArray.Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray.Capacity; ++probedSlotCount)
		{
			if (!IsLiveSlot(slotArray.Slots[position].Record.load(std::memory_order_relaxed)))
			{
				return position;
			}
			position = (position + 1) % slotArray.Capacity;
		}
		return slotArray.Capacity;
	}

	void InstrumentTablePrimaryKey::RebuildForInsertIfNeeded()
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		if ((usedSlotCount_ + 1) * 4 <= slotArray->Capacity * 3)
		{
			return;
		}
		Rebuild(slotArray->Capacity * 2);
	}

	void InstrumentTablePrimaryKey::Rebuild(std::size_t capacity)
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		SlotArray* slotArray = CreateSlotArray(capacity);
		ForEachRecord([slotArray](Instrument* record)
		{
			const std::size_t position = FindFreeSlotPositionIn(*slotArray, record);
			slotArray->Slots[position].Record.store(record, std::memory_order_relaxed);
			return true;
		});
		root_.store(slotArray, std::memory_order_release);
		usedSlotCount_ = rowCount_;
		// 记录已搬进新槽数组，旧数组只回收槽位本身
		epoch_.Retire(previousSlotArray, ReclaimSlotArray);
		epoch_.AdvanceAndReclaim();
	}

	InstrumentTablePrimaryKey::SlotArray* InstrumentTablePrimaryKey::CreateSlotArray(std::size_t capacity)
	{
		return new SlotArray{ new InstrumentSlot[capacity]{}, capacity };
	}

	void InstrumentTablePrimaryKey::ReclaimSlotArray(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void InstrumentTablePrimaryKey::ReclaimSlotArrayWithRecords(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		for (std::size_t position = 0; position < slotArray->Capacity; ++position)
		{
			Instrument* record = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (IsLiveSlot(record))
			{
				record->Deallocate();
			}
		}
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void InstrumentTablePrimaryKey::ReclaimRecord(void* object) noexcept
	{
		static_cast<Instrument*>(object)->Deallocate();
	}

	std::size_t PrimaryAccountTablePrimaryKey::HashKey(const PrimaryAccount* record) noexcept
	{
		return PrimaryAccountHashForPrimaryAccountPrimaryKey()(record);
	}

	PrimaryAccount* PrimaryAccountTablePrimaryKey::TombstoneSlot() noexcept
	{
		return reinterpret_cast<PrimaryAccount*>(static_cast<std::uintptr_t>(1));
	}

	bool PrimaryAccountTablePrimaryKey::IsLiveSlot(const PrimaryAccount* record) noexcept
	{
		return record != nullptr && record != TombstoneSlot();
	}

	PrimaryAccountTablePrimaryKey::iterator::iterator(const PrimaryAccountSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept
		: slotBegin_(slotBegin), slotCount_(slotCount), position_(position)
	{
		SkipUnoccupiedSlots();
	}

	PrimaryAccount* PrimaryAccountTablePrimaryKey::iterator::operator*() const noexcept
	{
		return slotBegin_[position_].Record.load(std::memory_order_acquire);
	}

	PrimaryAccountTablePrimaryKey::iterator& PrimaryAccountTablePrimaryKey::iterator::operator++() noexcept
	{
		++position_;
		SkipUnoccupiedSlots();
		return *this;
	}

	PrimaryAccountTablePrimaryKey::iterator PrimaryAccountTablePrimaryKey::iterator::operator++(int) noexcept
	{
		iterator previous = *this;
		++(*this);
		return previous;
	}

	bool PrimaryAccountTablePrimaryKey::iterator::operator==(const iterator& other) const noexcept
	{
		return slotBegin_ == other.slotBegin_ && position_ == other.position_;
	}

	bool PrimaryAccountTablePrimaryKey::iterator::operator!=(const iterator& other) const noexcept
	{
		return !(*this == other);
	}

	void PrimaryAccountTablePrimaryKey::iterator::SkipUnoccupiedSlots() noexcept
	{
		while (position_ < slotCount_ && !IsLiveSlot(slotBegin_[position_].Record.load(std::memory_order_relaxed)))
		{
			++position_;
		}
	}

	PrimaryAccountTablePrimaryKey::PrimaryAccountTablePrimaryKey(PrimaryAccountTable* tableOwner)
		: table_(tableOwner), root_(CreateSlotArray(InitialSlotCapacity))
	{
	}

	PrimaryAccountTablePrimaryKey::~PrimaryAccountTablePrimaryKey()
	{
		// 表已不可达，活记录与退休对象一次性归还，不必再等读者
		ReclaimSlotArrayWithRecords(root_.load(std::memory_order_acquire));
		root_.store(nullptr, std::memory_order_relaxed);
		epoch_.ReclaimAllWithoutReaders();
	}

	std::optional<PrimaryAccount> PrimaryAccountTablePrimaryKey::Select(const AccountIdType& PrimaryAccountId)
	{
		// 登记代数后按 seqlock 拷贝：写者在改同一行时版本为奇数，读者重来
		Rcu::ReaderEpoch::Guard guard(epoch_, Rcu::CurrentReaderIndex());
		PrimaryAccount keyRecord{};
		Utility::Strcpy(keyRecord.PrimaryAccountId, PrimaryAccountId);
		const PrimaryAccountSlot* slot = FindLiveSlot(&keyRecord);
		if (slot == nullptr)
		{
			return std::nullopt;
		}
		for (;;)
		{
			const std::uint64_t before = slot->Version.load(std::memory_order_acquire);
			if ((before & 1) != 0)
			{
				std::this_thread::yield();
				continue;
			}
			const PrimaryAccount* liveRecord = slot->Record.load(std::memory_order_acquire);
			if (!IsLiveSlot(liveRecord))
			{
				return std::nullopt;
			}
			PrimaryAccount copy{};
			std::memcpy(&copy, liveRecord, sizeof(PrimaryAccount));
			std::atomic_thread_fence(std::memory_order_acquire);
			if (slot->Version.load(std::memory_order_relaxed) == before)
			{
				return copy;
			}
			std::this_thread::yield();
		}
	}

	TableRangeLease<PrimaryAccountTablePrimaryKey::iterator> PrimaryAccountTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		const iterator first(slotArray->Slots, slotArray->Capacity, 0);
		const iterator last(slotArray->Slots, slotArray->Capacity, slotArray->Capacity);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}

	bool PrimaryAccountTablePrimaryKey::Insert(PrimaryAccount* const record)
	{
		RebuildForInsertIfNeeded();
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindFreeSlotPosition(record);
		if (slotArray->Slots[position].Record.load(std::memory_order_relaxed) == nullptr)
		{
			++usedSlotCount_;
		}
		slotArray->Slots[position].Record.store(record, std::memory_order_release);
		++rowCount_;
		return true;
	}

	void PrimaryAccountTablePrimaryKey::Erase(PrimaryAccount* const record)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(record);
		assert(position < slotArray->Capacity);
		slotArray->Slots[position].Record.store(TombstoneSlot(), std::memory_order_release);
		--rowCount_;
		epoch_.Retire(record, ReclaimRecord);
		epoch_.AdvanceAndReclaim();
	}

	bool PrimaryAccountTablePrimaryKey::CheckInsert(PrimaryAccount* const record)
	{
		return FindLiveSlot(record) == nullptr;
	}

	PrimaryAccount* PrimaryAccountTablePrimaryKey::Find(const PrimaryAccount* record)
	{
		const PrimaryAccountSlot* slot = FindLiveSlot(record);
		return slot == nullptr ? nullptr : const_cast<PrimaryAccount*>(slot->Record.load(std::memory_order_relaxed));
	}

	void PrimaryAccountTablePrimaryKey::Overwrite(PrimaryAccount* const liveRecord, const PrimaryAccount& newRecord)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(liveRecord);
		assert(position < slotArray->Capacity);
		PrimaryAccountSlot& slot = slotArray->Slots[position];
		const std::uint64_t before = slot.Version.load(std::memory_order_relaxed);
		slot.Version.store(before + 1, std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_release);
		// 主键字段不可变，不重写：避免与无锁查找读主键竞争
		std::memcpy(&liveRecord->PrimaryAccountName, &newRecord.PrimaryAccountName, sizeof(liveRecord->PrimaryAccountName));
		std::memcpy(&liveRecord->AccountClass, &newRecord.AccountClass, sizeof(liveRecord->AccountClass));
		std::memcpy(&liveRecord->BrokerPassword, &newRecord.BrokerPassword, sizeof(liveRecord->BrokerPassword));
		std::memcpy(&liveRecord->OfferId, &newRecord.OfferId, sizeof(liveRecord->OfferId));
		std::memcpy(&liveRecord->IsAllowLogin, &newRecord.IsAllowLogin, sizeof(liveRecord->IsAllowLogin));
		std::memcpy(&liveRecord->IsSimulateAccount, &newRecord.IsSimulateAccount, sizeof(liveRecord->IsSimulateAccount));
		std::memcpy(&liveRecord->LoginStatus, &newRecord.LoginStatus, sizeof(liveRecord->LoginStatus));
		std::memcpy(&liveRecord->InitStatus, &newRecord.InitStatus, sizeof(liveRecord->InitStatus));
		slot.Version.store(before + 2, std::memory_order_release);
	}

	void PrimaryAccountTablePrimaryKey::Clear()
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		root_.store(CreateSlotArray(InitialSlotCapacity), std::memory_order_release);
		rowCount_ = 0;
		usedSlotCount_ = 0;
		epoch_.Retire(previousSlotArray, ReclaimSlotArrayWithRecords);
		epoch_.AdvanceAndReclaim();
	}

	const PrimaryAccountSlot* PrimaryAccountTablePrimaryKey::FindLiveSlot(const PrimaryAccount* keyRecord) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		std::size_t position = HashKey(keyRecord) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const PrimaryAccountSlot* slot = &slotArray->Slots[position];
			const PrimaryAccount* candidate = slot->Record.load(std::memory_order_acquire);
			if (candidate == nullptr)
			{
				return nullptr;
			}
			if (IsLiveSlot(candidate) && PrimaryAccountEqualForPrimaryAccountPrimaryKey()(candidate, keyRecord))
			{
				return slot;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return nullptr;
	}

	std::size_t PrimaryAccountTablePrimaryKey::FindOccupiedSlotPosition(const PrimaryAccount* record) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		std::size_t position = HashKey(record) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const PrimaryAccount* candidate = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (candidate == nullptr)
			{
				break;
			}
			if (candidate == record)
			{
				return position;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return slotArray->Capacity;
	}

	std::size_t PrimaryAccountTablePrimaryKey::FindFreeSlotPosition(const PrimaryAccount* keyRecord) const
	{
		return FindFreeSlotPositionIn(*root_.load(std::memory_order_relaxed), keyRecord);
	}

	std::size_t PrimaryAccountTablePrimaryKey::FindFreeSlotPositionIn(const SlotArray& slotArray, const PrimaryAccount* keyRecord)
	{
		std::size_t position = HashKey(keyRecord) % slotArray.Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray.Capacity; ++probedSlotCount)
		{
			if (!IsLiveSlot(slotArray.Slots[position].Record.load(std::memory_order_relaxed)))
			{
				return position;
			}
			position = (position + 1) % slotArray.Capacity;
		}
		return slotArray.Capacity;
	}

	void PrimaryAccountTablePrimaryKey::RebuildForInsertIfNeeded()
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		if ((usedSlotCount_ + 1) * 4 <= slotArray->Capacity * 3)
		{
			return;
		}
		Rebuild(slotArray->Capacity * 2);
	}

	void PrimaryAccountTablePrimaryKey::Rebuild(std::size_t capacity)
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		SlotArray* slotArray = CreateSlotArray(capacity);
		ForEachRecord([slotArray](PrimaryAccount* record)
		{
			const std::size_t position = FindFreeSlotPositionIn(*slotArray, record);
			slotArray->Slots[position].Record.store(record, std::memory_order_relaxed);
			return true;
		});
		root_.store(slotArray, std::memory_order_release);
		usedSlotCount_ = rowCount_;
		// 记录已搬进新槽数组，旧数组只回收槽位本身
		epoch_.Retire(previousSlotArray, ReclaimSlotArray);
		epoch_.AdvanceAndReclaim();
	}

	PrimaryAccountTablePrimaryKey::SlotArray* PrimaryAccountTablePrimaryKey::CreateSlotArray(std::size_t capacity)
	{
		return new SlotArray{ new PrimaryAccountSlot[capacity]{}, capacity };
	}

	void PrimaryAccountTablePrimaryKey::ReclaimSlotArray(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void PrimaryAccountTablePrimaryKey::ReclaimSlotArrayWithRecords(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		for (std::size_t position = 0; position < slotArray->Capacity; ++position)
		{
			PrimaryAccount* record = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (IsLiveSlot(record))
			{
				record->Deallocate();
			}
		}
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void PrimaryAccountTablePrimaryKey::ReclaimRecord(void* object) noexcept
	{
		static_cast<PrimaryAccount*>(object)->Deallocate();
	}

	std::size_t AccountTablePrimaryKey::HashKey(const Account* record) noexcept
	{
		return AccountHashForAccountPrimaryKey()(record);
	}

	Account* AccountTablePrimaryKey::TombstoneSlot() noexcept
	{
		return reinterpret_cast<Account*>(static_cast<std::uintptr_t>(1));
	}

	bool AccountTablePrimaryKey::IsLiveSlot(const Account* record) noexcept
	{
		return record != nullptr && record != TombstoneSlot();
	}

	AccountTablePrimaryKey::iterator::iterator(const AccountSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept
		: slotBegin_(slotBegin), slotCount_(slotCount), position_(position)
	{
		SkipUnoccupiedSlots();
	}

	Account* AccountTablePrimaryKey::iterator::operator*() const noexcept
	{
		return slotBegin_[position_].Record.load(std::memory_order_acquire);
	}

	AccountTablePrimaryKey::iterator& AccountTablePrimaryKey::iterator::operator++() noexcept
	{
		++position_;
		SkipUnoccupiedSlots();
		return *this;
	}

	AccountTablePrimaryKey::iterator AccountTablePrimaryKey::iterator::operator++(int) noexcept
	{
		iterator previous = *this;
		++(*this);
		return previous;
	}

	bool AccountTablePrimaryKey::iterator::operator==(const iterator& other) const noexcept
	{
		return slotBegin_ == other.slotBegin_ && position_ == other.position_;
	}

	bool AccountTablePrimaryKey::iterator::operator!=(const iterator& other) const noexcept
	{
		return !(*this == other);
	}

	void AccountTablePrimaryKey::iterator::SkipUnoccupiedSlots() noexcept
	{
		while (position_ < slotCount_ && !IsLiveSlot(slotBegin_[position_].Record.load(std::memory_order_relaxed)))
		{
			++position_;
		}
	}

	AccountTablePrimaryKey::AccountTablePrimaryKey(AccountTable* tableOwner)
		: table_(tableOwner), root_(CreateSlotArray(InitialSlotCapacity))
	{
	}

	AccountTablePrimaryKey::~AccountTablePrimaryKey()
	{
		// 表已不可达，活记录与退休对象一次性归还，不必再等读者
		ReclaimSlotArrayWithRecords(root_.load(std::memory_order_acquire));
		root_.store(nullptr, std::memory_order_relaxed);
		epoch_.ReclaimAllWithoutReaders();
	}

	std::optional<Account> AccountTablePrimaryKey::Select(const AccountIdType& AccountId)
	{
		// 登记代数后按 seqlock 拷贝：写者在改同一行时版本为奇数，读者重来
		Rcu::ReaderEpoch::Guard guard(epoch_, Rcu::CurrentReaderIndex());
		Account keyRecord{};
		Utility::Strcpy(keyRecord.AccountId, AccountId);
		const AccountSlot* slot = FindLiveSlot(&keyRecord);
		if (slot == nullptr)
		{
			return std::nullopt;
		}
		for (;;)
		{
			const std::uint64_t before = slot->Version.load(std::memory_order_acquire);
			if ((before & 1) != 0)
			{
				std::this_thread::yield();
				continue;
			}
			const Account* liveRecord = slot->Record.load(std::memory_order_acquire);
			if (!IsLiveSlot(liveRecord))
			{
				return std::nullopt;
			}
			Account copy{};
			std::memcpy(&copy, liveRecord, sizeof(Account));
			std::atomic_thread_fence(std::memory_order_acquire);
			if (slot->Version.load(std::memory_order_relaxed) == before)
			{
				return copy;
			}
			std::this_thread::yield();
		}
	}

	TableRangeLease<AccountTablePrimaryKey::iterator> AccountTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		const iterator first(slotArray->Slots, slotArray->Capacity, 0);
		const iterator last(slotArray->Slots, slotArray->Capacity, slotArray->Capacity);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}

	bool AccountTablePrimaryKey::Insert(Account* const record)
	{
		RebuildForInsertIfNeeded();
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindFreeSlotPosition(record);
		if (slotArray->Slots[position].Record.load(std::memory_order_relaxed) == nullptr)
		{
			++usedSlotCount_;
		}
		slotArray->Slots[position].Record.store(record, std::memory_order_release);
		++rowCount_;
		return true;
	}

	void AccountTablePrimaryKey::Erase(Account* const record)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(record);
		assert(position < slotArray->Capacity);
		slotArray->Slots[position].Record.store(TombstoneSlot(), std::memory_order_release);
		--rowCount_;
		epoch_.Retire(record, ReclaimRecord);
		epoch_.AdvanceAndReclaim();
	}

	bool AccountTablePrimaryKey::CheckInsert(Account* const record)
	{
		return FindLiveSlot(record) == nullptr;
	}

	Account* AccountTablePrimaryKey::Find(const Account* record)
	{
		const AccountSlot* slot = FindLiveSlot(record);
		return slot == nullptr ? nullptr : const_cast<Account*>(slot->Record.load(std::memory_order_relaxed));
	}

	void AccountTablePrimaryKey::Overwrite(Account* const liveRecord, const Account& newRecord)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(liveRecord);
		assert(position < slotArray->Capacity);
		AccountSlot& slot = slotArray->Slots[position];
		const std::uint64_t before = slot.Version.load(std::memory_order_relaxed);
		slot.Version.store(before + 1, std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_release);
		// 主键字段不可变，不重写：避免与无锁查找读主键竞争
		std::memcpy(&liveRecord->AccountName, &newRecord.AccountName, sizeof(liveRecord->AccountName));
		std::memcpy(&liveRecord->AccountType, &newRecord.AccountType, sizeof(liveRecord->AccountType));
		std::memcpy(&liveRecord->AccountStatus, &newRecord.AccountStatus, sizeof(liveRecord->AccountStatus));
		std::memcpy(&liveRecord->Password, &newRecord.Password, sizeof(liveRecord->Password));
		std::memcpy(&liveRecord->TradeGroupId, &newRecord.TradeGroupId, sizeof(liveRecord->TradeGroupId));
		std::memcpy(&liveRecord->RiskGroupId, &newRecord.RiskGroupId, sizeof(liveRecord->RiskGroupId));
		std::memcpy(&liveRecord->CommissionGroupId, &newRecord.CommissionGroupId, sizeof(liveRecord->CommissionGroupId));
		slot.Version.store(before + 2, std::memory_order_release);
	}

	void AccountTablePrimaryKey::Clear()
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		root_.store(CreateSlotArray(InitialSlotCapacity), std::memory_order_release);
		rowCount_ = 0;
		usedSlotCount_ = 0;
		epoch_.Retire(previousSlotArray, ReclaimSlotArrayWithRecords);
		epoch_.AdvanceAndReclaim();
	}

	const AccountSlot* AccountTablePrimaryKey::FindLiveSlot(const Account* keyRecord) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		std::size_t position = HashKey(keyRecord) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const AccountSlot* slot = &slotArray->Slots[position];
			const Account* candidate = slot->Record.load(std::memory_order_acquire);
			if (candidate == nullptr)
			{
				return nullptr;
			}
			if (IsLiveSlot(candidate) && AccountEqualForAccountPrimaryKey()(candidate, keyRecord))
			{
				return slot;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return nullptr;
	}

	std::size_t AccountTablePrimaryKey::FindOccupiedSlotPosition(const Account* record) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		std::size_t position = HashKey(record) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const Account* candidate = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (candidate == nullptr)
			{
				break;
			}
			if (candidate == record)
			{
				return position;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return slotArray->Capacity;
	}

	std::size_t AccountTablePrimaryKey::FindFreeSlotPosition(const Account* keyRecord) const
	{
		return FindFreeSlotPositionIn(*root_.load(std::memory_order_relaxed), keyRecord);
	}

	std::size_t AccountTablePrimaryKey::FindFreeSlotPositionIn(const SlotArray& slotArray, const Account* keyRecord)
	{
		std::size_t position = HashKey(keyRecord) % slotArray.Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray.Capacity; ++probedSlotCount)
		{
			if (!IsLiveSlot(slotArray.Slots[position].Record.load(std::memory_order_relaxed)))
			{
				return position;
			}
			position = (position + 1) % slotArray.Capacity;
		}
		return slotArray.Capacity;
	}

	void AccountTablePrimaryKey::RebuildForInsertIfNeeded()
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		if ((usedSlotCount_ + 1) * 4 <= slotArray->Capacity * 3)
		{
			return;
		}
		Rebuild(slotArray->Capacity * 2);
	}

	void AccountTablePrimaryKey::Rebuild(std::size_t capacity)
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		SlotArray* slotArray = CreateSlotArray(capacity);
		ForEachRecord([slotArray](Account* record)
		{
			const std::size_t position = FindFreeSlotPositionIn(*slotArray, record);
			slotArray->Slots[position].Record.store(record, std::memory_order_relaxed);
			return true;
		});
		root_.store(slotArray, std::memory_order_release);
		usedSlotCount_ = rowCount_;
		// 记录已搬进新槽数组，旧数组只回收槽位本身
		epoch_.Retire(previousSlotArray, ReclaimSlotArray);
		epoch_.AdvanceAndReclaim();
	}

	AccountTablePrimaryKey::SlotArray* AccountTablePrimaryKey::CreateSlotArray(std::size_t capacity)
	{
		return new SlotArray{ new AccountSlot[capacity]{}, capacity };
	}

	void AccountTablePrimaryKey::ReclaimSlotArray(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void AccountTablePrimaryKey::ReclaimSlotArrayWithRecords(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		for (std::size_t position = 0; position < slotArray->Capacity; ++position)
		{
			Account* record = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (IsLiveSlot(record))
			{
				record->Deallocate();
			}
		}
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void AccountTablePrimaryKey::ReclaimRecord(void* object) noexcept
	{
		static_cast<Account*>(object)->Deallocate();
	}

	std::size_t CapitalTablePrimaryKey::HashKey(const Capital* record) noexcept
	{
		return CapitalHashForCapitalPrimaryKey()(record);
	}

	Capital* CapitalTablePrimaryKey::TombstoneSlot() noexcept
	{
		return reinterpret_cast<Capital*>(static_cast<std::uintptr_t>(1));
	}

	bool CapitalTablePrimaryKey::IsLiveSlot(const Capital* record) noexcept
	{
		return record != nullptr && record != TombstoneSlot();
	}

	CapitalTablePrimaryKey::iterator::iterator(const CapitalSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept
		: slotBegin_(slotBegin), slotCount_(slotCount), position_(position)
	{
		SkipUnoccupiedSlots();
	}

	Capital* CapitalTablePrimaryKey::iterator::operator*() const noexcept
	{
		return slotBegin_[position_].Record.load(std::memory_order_acquire);
	}

	CapitalTablePrimaryKey::iterator& CapitalTablePrimaryKey::iterator::operator++() noexcept
	{
		++position_;
		SkipUnoccupiedSlots();
		return *this;
	}

	CapitalTablePrimaryKey::iterator CapitalTablePrimaryKey::iterator::operator++(int) noexcept
	{
		iterator previous = *this;
		++(*this);
		return previous;
	}

	bool CapitalTablePrimaryKey::iterator::operator==(const iterator& other) const noexcept
	{
		return slotBegin_ == other.slotBegin_ && position_ == other.position_;
	}

	bool CapitalTablePrimaryKey::iterator::operator!=(const iterator& other) const noexcept
	{
		return !(*this == other);
	}

	void CapitalTablePrimaryKey::iterator::SkipUnoccupiedSlots() noexcept
	{
		while (position_ < slotCount_ && !IsLiveSlot(slotBegin_[position_].Record.load(std::memory_order_relaxed)))
		{
			++position_;
		}
	}

	CapitalTablePrimaryKey::CapitalTablePrimaryKey(CapitalTable* tableOwner)
		: table_(tableOwner), root_(CreateSlotArray(InitialSlotCapacity))
	{
	}

	CapitalTablePrimaryKey::~CapitalTablePrimaryKey()
	{
		// 表已不可达，活记录与退休对象一次性归还，不必再等读者
		ReclaimSlotArrayWithRecords(root_.load(std::memory_order_acquire));
		root_.store(nullptr, std::memory_order_relaxed);
		epoch_.ReclaimAllWithoutReaders();
	}

	std::optional<Capital> CapitalTablePrimaryKey::Select(const DateType& TradingDay, const AccountIdType& AccountId)
	{
		// 登记代数后按 seqlock 拷贝：写者在改同一行时版本为奇数，读者重来
		Rcu::ReaderEpoch::Guard guard(epoch_, Rcu::CurrentReaderIndex());
		Capital keyRecord{};
		Utility::Strcpy(keyRecord.TradingDay, TradingDay);
		Utility::Strcpy(keyRecord.AccountId, AccountId);
		const CapitalSlot* slot = FindLiveSlot(&keyRecord);
		if (slot == nullptr)
		{
			return std::nullopt;
		}
		for (;;)
		{
			const std::uint64_t before = slot->Version.load(std::memory_order_acquire);
			if ((before & 1) != 0)
			{
				std::this_thread::yield();
				continue;
			}
			const Capital* liveRecord = slot->Record.load(std::memory_order_acquire);
			if (!IsLiveSlot(liveRecord))
			{
				return std::nullopt;
			}
			Capital copy{};
			std::memcpy(&copy, liveRecord, sizeof(Capital));
			std::atomic_thread_fence(std::memory_order_acquire);
			if (slot->Version.load(std::memory_order_relaxed) == before)
			{
				return copy;
			}
			std::this_thread::yield();
		}
	}

	TableRangeLease<CapitalTablePrimaryKey::iterator> CapitalTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		const iterator first(slotArray->Slots, slotArray->Capacity, 0);
		const iterator last(slotArray->Slots, slotArray->Capacity, slotArray->Capacity);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}

	bool CapitalTablePrimaryKey::Insert(Capital* const record)
	{
		RebuildForInsertIfNeeded();
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindFreeSlotPosition(record);
		if (slotArray->Slots[position].Record.load(std::memory_order_relaxed) == nullptr)
		{
			++usedSlotCount_;
		}
		slotArray->Slots[position].Record.store(record, std::memory_order_release);
		++rowCount_;
		return true;
	}

	void CapitalTablePrimaryKey::Erase(Capital* const record)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(record);
		assert(position < slotArray->Capacity);
		slotArray->Slots[position].Record.store(TombstoneSlot(), std::memory_order_release);
		--rowCount_;
		epoch_.Retire(record, ReclaimRecord);
		epoch_.AdvanceAndReclaim();
	}

	bool CapitalTablePrimaryKey::CheckInsert(Capital* const record)
	{
		return FindLiveSlot(record) == nullptr;
	}

	Capital* CapitalTablePrimaryKey::Find(const Capital* record)
	{
		const CapitalSlot* slot = FindLiveSlot(record);
		return slot == nullptr ? nullptr : const_cast<Capital*>(slot->Record.load(std::memory_order_relaxed));
	}

	void CapitalTablePrimaryKey::Overwrite(Capital* const liveRecord, const Capital& newRecord)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(liveRecord);
		assert(position < slotArray->Capacity);
		CapitalSlot& slot = slotArray->Slots[position];
		const std::uint64_t before = slot.Version.load(std::memory_order_relaxed);
		slot.Version.store(before + 1, std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_release);
		// 主键字段不可变，不重写：避免与无锁查找读主键竞争
		std::memcpy(&liveRecord->AccountType, &newRecord.AccountType, sizeof(liveRecord->AccountType));
		std::memcpy(&liveRecord->Balance, &newRecord.Balance, sizeof(liveRecord->Balance));
		std::memcpy(&liveRecord->PreBalance, &newRecord.PreBalance, sizeof(liveRecord->PreBalance));
		std::memcpy(&liveRecord->Available, &newRecord.Available, sizeof(liveRecord->Available));
		std::memcpy(&liveRecord->MarketValue, &newRecord.MarketValue, sizeof(liveRecord->MarketValue));
		std::memcpy(&liveRecord->CashIn, &newRecord.CashIn, sizeof(liveRecord->CashIn));
		std::memcpy(&liveRecord->CashOut, &newRecord.CashOut, sizeof(liveRecord->CashOut));
		std::memcpy(&liveRecord->Margin, &newRecord.Margin, sizeof(liveRecord->Margin));
		std::memcpy(&liveRecord->Commission, &newRecord.Commission, sizeof(liveRecord->Commission));
		std::memcpy(&liveRecord->FrozenCash, &newRecord.FrozenCash, sizeof(liveRecord->FrozenCash));
		std::memcpy(&liveRecord->FrozenMargin, &newRecord.FrozenMargin, sizeof(liveRecord->FrozenMargin));
		std::memcpy(&liveRecord->FrozenCommission, &newRecord.FrozenCommission, sizeof(liveRecord->FrozenCommission));
		std::memcpy(&liveRecord->CloseProfitByDate, &newRecord.CloseProfitByDate, sizeof(liveRecord->CloseProfitByDate));
		std::memcpy(&liveRecord->CloseProfitByTrade, &newRecord.CloseProfitByTrade, sizeof(liveRecord->CloseProfitByTrade));
		std::memcpy(&liveRecord->PositionProfitByDate, &newRecord.PositionProfitByDate, sizeof(liveRecord->PositionProfitByDate));
		std::memcpy(&liveRecord->PositionProfitByTrade, &newRecord.PositionProfitByTrade, sizeof(liveRecord->PositionProfitByTrade));
		std::memcpy(&liveRecord->Deposit, &newRecord.Deposit, sizeof(liveRecord->Deposit));
		std::memcpy(&liveRecord->Withdraw, &newRecord.Withdraw, sizeof(liveRecord->Withdraw));
		slot.Version.store(before + 2, std::memory_order_release);
	}

	void CapitalTablePrimaryKey::Clear()
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		root_.store(CreateSlotArray(InitialSlotCapacity), std::memory_order_release);
		rowCount_ = 0;
		usedSlotCount_ = 0;
		epoch_.Retire(previousSlotArray, ReclaimSlotArrayWithRecords);
		epoch_.AdvanceAndReclaim();
	}

	const CapitalSlot* CapitalTablePrimaryKey::FindLiveSlot(const Capital* keyRecord) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		std::size_t position = HashKey(keyRecord) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const CapitalSlot* slot = &slotArray->Slots[position];
			const Capital* candidate = slot->Record.load(std::memory_order_acquire);
			if (candidate == nullptr)
			{
				return nullptr;
			}
			if (IsLiveSlot(candidate) && CapitalEqualForCapitalPrimaryKey()(candidate, keyRecord))
			{
				return slot;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return nullptr;
	}

	std::size_t CapitalTablePrimaryKey::FindOccupiedSlotPosition(const Capital* record) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		std::size_t position = HashKey(record) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const Capital* candidate = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (candidate == nullptr)
			{
				break;
			}
			if (candidate == record)
			{
				return position;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return slotArray->Capacity;
	}

	std::size_t CapitalTablePrimaryKey::FindFreeSlotPosition(const Capital* keyRecord) const
	{
		return FindFreeSlotPositionIn(*root_.load(std::memory_order_relaxed), keyRecord);
	}

	std::size_t CapitalTablePrimaryKey::FindFreeSlotPositionIn(const SlotArray& slotArray, const Capital* keyRecord)
	{
		std::size_t position = HashKey(keyRecord) % slotArray.Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray.Capacity; ++probedSlotCount)
		{
			if (!IsLiveSlot(slotArray.Slots[position].Record.load(std::memory_order_relaxed)))
			{
				return position;
			}
			position = (position + 1) % slotArray.Capacity;
		}
		return slotArray.Capacity;
	}

	void CapitalTablePrimaryKey::RebuildForInsertIfNeeded()
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		if ((usedSlotCount_ + 1) * 4 <= slotArray->Capacity * 3)
		{
			return;
		}
		Rebuild(slotArray->Capacity * 2);
	}

	void CapitalTablePrimaryKey::Rebuild(std::size_t capacity)
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		SlotArray* slotArray = CreateSlotArray(capacity);
		ForEachRecord([slotArray](Capital* record)
		{
			const std::size_t position = FindFreeSlotPositionIn(*slotArray, record);
			slotArray->Slots[position].Record.store(record, std::memory_order_relaxed);
			return true;
		});
		root_.store(slotArray, std::memory_order_release);
		usedSlotCount_ = rowCount_;
		// 记录已搬进新槽数组，旧数组只回收槽位本身
		epoch_.Retire(previousSlotArray, ReclaimSlotArray);
		epoch_.AdvanceAndReclaim();
	}

	CapitalTablePrimaryKey::SlotArray* CapitalTablePrimaryKey::CreateSlotArray(std::size_t capacity)
	{
		return new SlotArray{ new CapitalSlot[capacity]{}, capacity };
	}

	void CapitalTablePrimaryKey::ReclaimSlotArray(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void CapitalTablePrimaryKey::ReclaimSlotArrayWithRecords(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		for (std::size_t position = 0; position < slotArray->Capacity; ++position)
		{
			Capital* record = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (IsLiveSlot(record))
			{
				record->Deallocate();
			}
		}
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void CapitalTablePrimaryKey::ReclaimRecord(void* object) noexcept
	{
		static_cast<Capital*>(object)->Deallocate();
	}

	std::size_t PositionTablePrimaryKey::HashKey(const Position* record) noexcept
	{
		return PositionHashForPositionPrimaryKey()(record);
	}

	Position* PositionTablePrimaryKey::TombstoneSlot() noexcept
	{
		return reinterpret_cast<Position*>(static_cast<std::uintptr_t>(1));
	}

	bool PositionTablePrimaryKey::IsLiveSlot(const Position* record) noexcept
	{
		return record != nullptr && record != TombstoneSlot();
	}

	PositionTablePrimaryKey::iterator::iterator(const PositionSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept
		: slotBegin_(slotBegin), slotCount_(slotCount), position_(position)
	{
		SkipUnoccupiedSlots();
	}

	Position* PositionTablePrimaryKey::iterator::operator*() const noexcept
	{
		return slotBegin_[position_].Record.load(std::memory_order_acquire);
	}

	PositionTablePrimaryKey::iterator& PositionTablePrimaryKey::iterator::operator++() noexcept
	{
		++position_;
		SkipUnoccupiedSlots();
		return *this;
	}

	PositionTablePrimaryKey::iterator PositionTablePrimaryKey::iterator::operator++(int) noexcept
	{
		iterator previous = *this;
		++(*this);
		return previous;
	}

	bool PositionTablePrimaryKey::iterator::operator==(const iterator& other) const noexcept
	{
		return slotBegin_ == other.slotBegin_ && position_ == other.position_;
	}

	bool PositionTablePrimaryKey::iterator::operator!=(const iterator& other) const noexcept
	{
		return !(*this == other);
	}

	void PositionTablePrimaryKey::iterator::SkipUnoccupiedSlots() noexcept
	{
		while (position_ < slotCount_ && !IsLiveSlot(slotBegin_[position_].Record.load(std::memory_order_relaxed)))
		{
			++position_;
		}
	}

	PositionTablePrimaryKey::PositionTablePrimaryKey(PositionTable* tableOwner)
		: table_(tableOwner), root_(CreateSlotArray(InitialSlotCapacity))
	{
	}

	PositionTablePrimaryKey::~PositionTablePrimaryKey()
	{
		// 表已不可达，活记录与退休对象一次性归还，不必再等读者
		ReclaimSlotArrayWithRecords(root_.load(std::memory_order_acquire));
		root_.store(nullptr, std::memory_order_relaxed);
		epoch_.ReclaimAllWithoutReaders();
	}

	std::optional<Position> PositionTablePrimaryKey::Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection)
	{
		// 登记代数后按 seqlock 拷贝：写者在改同一行时版本为奇数，读者重来
		Rcu::ReaderEpoch::Guard guard(epoch_, Rcu::CurrentReaderIndex());
		Position keyRecord{};
		Utility::Strcpy(keyRecord.TradingDay, TradingDay);
		Utility::Strcpy(keyRecord.AccountId, AccountId);
		Utility::Strcpy(keyRecord.ExchangeId, ExchangeId);
		Utility::Strcpy(keyRecord.InstrumentId, InstrumentId);
		keyRecord.PosiDirection = PosiDirection;
		const PositionSlot* slot = FindLiveSlot(&keyRecord);
		if (slot == nullptr)
		{
			return std::nullopt;
		}
		for (;;)
		{
			const std::uint64_t before = slot->Version.load(std::memory_order_acquire);
			if ((before & 1) != 0)
			{
				std::this_thread::yield();
				continue;
			}
			const Position* liveRecord = slot->Record.load(std::memory_order_acquire);
			if (!IsLiveSlot(liveRecord))
			{
				return std::nullopt;
			}
			Position copy{};
			std::memcpy(&copy, liveRecord, sizeof(Position));
			std::atomic_thread_fence(std::memory_order_acquire);
			if (slot->Version.load(std::memory_order_relaxed) == before)
			{
				return copy;
			}
			std::this_thread::yield();
		}
	}

	TableRangeLease<PositionTablePrimaryKey::iterator> PositionTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		const iterator first(slotArray->Slots, slotArray->Capacity, 0);
		const iterator last(slotArray->Slots, slotArray->Capacity, slotArray->Capacity);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}

	bool PositionTablePrimaryKey::Insert(Position* const record)
	{
		RebuildForInsertIfNeeded();
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindFreeSlotPosition(record);
		if (slotArray->Slots[position].Record.load(std::memory_order_relaxed) == nullptr)
		{
			++usedSlotCount_;
		}
		slotArray->Slots[position].Record.store(record, std::memory_order_release);
		++rowCount_;
		return true;
	}

	void PositionTablePrimaryKey::Erase(Position* const record)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(record);
		assert(position < slotArray->Capacity);
		slotArray->Slots[position].Record.store(TombstoneSlot(), std::memory_order_release);
		--rowCount_;
		epoch_.Retire(record, ReclaimRecord);
		epoch_.AdvanceAndReclaim();
	}

	bool PositionTablePrimaryKey::CheckInsert(Position* const record)
	{
		return FindLiveSlot(record) == nullptr;
	}

	Position* PositionTablePrimaryKey::Find(const Position* record)
	{
		const PositionSlot* slot = FindLiveSlot(record);
		return slot == nullptr ? nullptr : const_cast<Position*>(slot->Record.load(std::memory_order_relaxed));
	}

	void PositionTablePrimaryKey::Overwrite(Position* const liveRecord, const Position& newRecord)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(liveRecord);
		assert(position < slotArray->Capacity);
		PositionSlot& slot = slotArray->Slots[position];
		const std::uint64_t before = slot.Version.load(std::memory_order_relaxed);
		slot.Version.store(before + 1, std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_release);
		// 主键字段不可变，不重写：避免与无锁查找读主键竞争
		std::memcpy(&liveRecord->AccountType, &newRecord.AccountType, sizeof(liveRecord->AccountType));
		std::memcpy(&liveRecord->ProductClass, &newRecord.ProductClass, sizeof(liveRecord->ProductClass));
		std::memcpy(&liveRecord->TotalPosition, &newRecord.TotalPosition, sizeof(liveRecord->TotalPosition));
		std::memcpy(&liveRecord->PositionFrozen, &newRecord.PositionFrozen, sizeof(liveRecord->PositionFrozen));
		std::memcpy(&liveRecord->TodayPosition, &newRecord.TodayPosition, sizeof(liveRecord->TodayPosition));
		std::memcpy(&liveRecord->MarketValue, &newRecord.MarketValue, sizeof(liveRecord->MarketValue));
		std::memcpy(&liveRecord->CashIn, &newRecord.CashIn, sizeof(liveRecord->CashIn));
		std::memcpy(&liveRecord->CashOut, &newRecord.CashOut, sizeof(liveRecord->CashOut));
		std::memcpy(&liveRecord->Margin, &newRecord.Margin, sizeof(liveRecord->Margin));
		std::memcpy(&liveRecord->Commission, &newRecord.Commission, sizeof(liveRecord->Commission));
		std::memcpy(&liveRecord->FrozenCash, &newRecord.FrozenCash, sizeof(liveRecord->FrozenCash));
		std::memcpy(&liveRecord->FrozenMargin, &newRecord.FrozenMargin, sizeof(liveRecord->FrozenMargin));
		std::memcpy(&liveRecord->FrozenCommission, &newRecord.FrozenCommission, sizeof(liveRecord->FrozenCommission));
		std::memcpy(&liveRecord->VolumeMultiple, &newRecord.VolumeMultiple, sizeof(liveRecord->VolumeMultiple));
		std::memcpy(&liveRecord->CloseProfitByDate, &newRecord.CloseProfitByDate, sizeof(liveRecord->CloseProfitByDate));
		std::memcpy(&liveRecord->CloseProfitByTrade, &newRecord.CloseProfitByTrade, sizeof(liveRecord->CloseProfitByTrade));
		std::memcpy(&liveRecord->PositionProfitByDate, &newRecord.PositionProfitByDate, sizeof(liveRecord->PositionProfitByDate));
		std::memcpy(&liveRecord->PositionProfitByTrade, &newRecord.PositionProfitByTrade, sizeof(liveRecord->PositionProfitByTrade));
		std::memcpy(&liveRecord->SettlementPrice, &newRecord.SettlementPrice, sizeof(liveRecord->SettlementPrice));
		std::memcpy(&liveRecord->PreSettlementPrice, &newRecord.PreSettlementPrice, sizeof(liveRecord->PreSettlementPrice));
		slot.Version.store(before + 2, std::memory_order_release);
	}

	void PositionTablePrimaryKey::Clear()
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		root_.store(CreateSlotArray(InitialSlotCapacity), std::memory_order_release);
		rowCount_ = 0;
		usedSlotCount_ = 0;
		epoch_.Retire(previousSlotArray, ReclaimSlotArrayWithRecords);
		epoch_.AdvanceAndReclaim();
	}

	const PositionSlot* PositionTablePrimaryKey::FindLiveSlot(const Position* keyRecord) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		std::size_t position = HashKey(keyRecord) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const PositionSlot* slot = &slotArray->Slots[position];
			const Position* candidate = slot->Record.load(std::memory_order_acquire);
			if (candidate == nullptr)
			{
				return nullptr;
			}
			if (IsLiveSlot(candidate) && PositionEqualForPositionPrimaryKey()(candidate, keyRecord))
			{
				return slot;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return nullptr;
	}

	std::size_t PositionTablePrimaryKey::FindOccupiedSlotPosition(const Position* record) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		std::size_t position = HashKey(record) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const Position* candidate = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (candidate == nullptr)
			{
				break;
			}
			if (candidate == record)
			{
				return position;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return slotArray->Capacity;
	}

	std::size_t PositionTablePrimaryKey::FindFreeSlotPosition(const Position* keyRecord) const
	{
		return FindFreeSlotPositionIn(*root_.load(std::memory_order_relaxed), keyRecord);
	}

	std::size_t PositionTablePrimaryKey::FindFreeSlotPositionIn(const SlotArray& slotArray, const Position* keyRecord)
	{
		std::size_t position = HashKey(keyRecord) % slotArray.Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray.Capacity; ++probedSlotCount)
		{
			if (!IsLiveSlot(slotArray.Slots[position].Record.load(std::memory_order_relaxed)))
			{
				return position;
			}
			position = (position + 1) % slotArray.Capacity;
		}
		return slotArray.Capacity;
	}

	void PositionTablePrimaryKey::RebuildForInsertIfNeeded()
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		if ((usedSlotCount_ + 1) * 4 <= slotArray->Capacity * 3)
		{
			return;
		}
		Rebuild(slotArray->Capacity * 2);
	}

	void PositionTablePrimaryKey::Rebuild(std::size_t capacity)
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		SlotArray* slotArray = CreateSlotArray(capacity);
		ForEachRecord([slotArray](Position* record)
		{
			const std::size_t position = FindFreeSlotPositionIn(*slotArray, record);
			slotArray->Slots[position].Record.store(record, std::memory_order_relaxed);
			return true;
		});
		root_.store(slotArray, std::memory_order_release);
		usedSlotCount_ = rowCount_;
		// 记录已搬进新槽数组，旧数组只回收槽位本身
		epoch_.Retire(previousSlotArray, ReclaimSlotArray);
		epoch_.AdvanceAndReclaim();
	}

	PositionTablePrimaryKey::SlotArray* PositionTablePrimaryKey::CreateSlotArray(std::size_t capacity)
	{
		return new SlotArray{ new PositionSlot[capacity]{}, capacity };
	}

	void PositionTablePrimaryKey::ReclaimSlotArray(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void PositionTablePrimaryKey::ReclaimSlotArrayWithRecords(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		for (std::size_t position = 0; position < slotArray->Capacity; ++position)
		{
			Position* record = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (IsLiveSlot(record))
			{
				record->Deallocate();
			}
		}
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void PositionTablePrimaryKey::ReclaimRecord(void* object) noexcept
	{
		static_cast<Position*>(object)->Deallocate();
	}

	std::size_t PositionDetailTablePrimaryKey::HashKey(const PositionDetail* record) noexcept
	{
		return PositionDetailHashForPositionDetailPrimaryKey()(record);
	}

	PositionDetail* PositionDetailTablePrimaryKey::TombstoneSlot() noexcept
	{
		return reinterpret_cast<PositionDetail*>(static_cast<std::uintptr_t>(1));
	}

	bool PositionDetailTablePrimaryKey::IsLiveSlot(const PositionDetail* record) noexcept
	{
		return record != nullptr && record != TombstoneSlot();
	}

	PositionDetailTablePrimaryKey::iterator::iterator(const PositionDetailSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept
		: slotBegin_(slotBegin), slotCount_(slotCount), position_(position)
	{
		SkipUnoccupiedSlots();
	}

	PositionDetail* PositionDetailTablePrimaryKey::iterator::operator*() const noexcept
	{
		return slotBegin_[position_].Record.load(std::memory_order_acquire);
	}

	PositionDetailTablePrimaryKey::iterator& PositionDetailTablePrimaryKey::iterator::operator++() noexcept
	{
		++position_;
		SkipUnoccupiedSlots();
		return *this;
	}

	PositionDetailTablePrimaryKey::iterator PositionDetailTablePrimaryKey::iterator::operator++(int) noexcept
	{
		iterator previous = *this;
		++(*this);
		return previous;
	}

	bool PositionDetailTablePrimaryKey::iterator::operator==(const iterator& other) const noexcept
	{
		return slotBegin_ == other.slotBegin_ && position_ == other.position_;
	}

	bool PositionDetailTablePrimaryKey::iterator::operator!=(const iterator& other) const noexcept
	{
		return !(*this == other);
	}

	void PositionDetailTablePrimaryKey::iterator::SkipUnoccupiedSlots() noexcept
	{
		while (position_ < slotCount_ && !IsLiveSlot(slotBegin_[position_].Record.load(std::memory_order_relaxed)))
		{
			++position_;
		}
	}

	PositionDetailTablePrimaryKey::PositionDetailTablePrimaryKey(PositionDetailTable* tableOwner)
		: table_(tableOwner), root_(CreateSlotArray(InitialSlotCapacity))
	{
	}

	PositionDetailTablePrimaryKey::~PositionDetailTablePrimaryKey()
	{
		// 表已不可达，活记录与退休对象一次性归还，不必再等读者
		ReclaimSlotArrayWithRecords(root_.load(std::memory_order_acquire));
		root_.store(nullptr, std::memory_order_relaxed);
		epoch_.ReclaimAllWithoutReaders();
	}

	std::optional<PositionDetail> PositionDetailTablePrimaryKey::Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection, const DateType& OpenDate, const TradeIdType& TradeId)
	{
		// 登记代数后按 seqlock 拷贝：写者在改同一行时版本为奇数，读者重来
		Rcu::ReaderEpoch::Guard guard(epoch_, Rcu::CurrentReaderIndex());
		PositionDetail keyRecord{};
		Utility::Strcpy(keyRecord.TradingDay, TradingDay);
		Utility::Strcpy(keyRecord.AccountId, AccountId);
		Utility::Strcpy(keyRecord.ExchangeId, ExchangeId);
		Utility::Strcpy(keyRecord.InstrumentId, InstrumentId);
		keyRecord.PosiDirection = PosiDirection;
		Utility::Strcpy(keyRecord.OpenDate, OpenDate);
		Utility::Strcpy(keyRecord.TradeId, TradeId);
		const PositionDetailSlot* slot = FindLiveSlot(&keyRecord);
		if (slot == nullptr)
		{
			return std::nullopt;
		}
		for (;;)
		{
			const std::uint64_t before = slot->Version.load(std::memory_order_acquire);
			if ((before & 1) != 0)
			{
				std::this_thread::yield();
				continue;
			}
			const PositionDetail* liveRecord = slot->Record.load(std::memory_order_acquire);
			if (!IsLiveSlot(liveRecord))
			{
				return std::nullopt;
			}
			PositionDetail copy{};
			std::memcpy(&copy, liveRecord, sizeof(PositionDetail));
			std::atomic_thread_fence(std::memory_order_acquire);
			if (slot->Version.load(std::memory_order_relaxed) == before)
			{
				return copy;
			}
			std::this_thread::yield();
		}
	}

	TableRangeLease<PositionDetailTablePrimaryKey::iterator> PositionDetailTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		const iterator first(slotArray->Slots, slotArray->Capacity, 0);
		const iterator last(slotArray->Slots, slotArray->Capacity, slotArray->Capacity);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}

	bool PositionDetailTablePrimaryKey::Insert(PositionDetail* const record)
	{
		RebuildForInsertIfNeeded();
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindFreeSlotPosition(record);
		if (slotArray->Slots[position].Record.load(std::memory_order_relaxed) == nullptr)
		{
			++usedSlotCount_;
		}
		slotArray->Slots[position].Record.store(record, std::memory_order_release);
		++rowCount_;
		return true;
	}

	void PositionDetailTablePrimaryKey::Erase(PositionDetail* const record)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(record);
		assert(position < slotArray->Capacity);
		slotArray->Slots[position].Record.store(TombstoneSlot(), std::memory_order_release);
		--rowCount_;
		epoch_.Retire(record, ReclaimRecord);
		epoch_.AdvanceAndReclaim();
	}

	bool PositionDetailTablePrimaryKey::CheckInsert(PositionDetail* const record)
	{
		return FindLiveSlot(record) == nullptr;
	}

	PositionDetail* PositionDetailTablePrimaryKey::Find(const PositionDetail* record)
	{
		const PositionDetailSlot* slot = FindLiveSlot(record);
		return slot == nullptr ? nullptr : const_cast<PositionDetail*>(slot->Record.load(std::memory_order_relaxed));
	}

	void PositionDetailTablePrimaryKey::Overwrite(PositionDetail* const liveRecord, const PositionDetail& newRecord)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(liveRecord);
		assert(position < slotArray->Capacity);
		PositionDetailSlot& slot = slotArray->Slots[position];
		const std::uint64_t before = slot.Version.load(std::memory_order_relaxed);
		slot.Version.store(before + 1, std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_release);
		// 主键字段不可变，不重写：避免与无锁查找读主键竞争
		std::memcpy(&liveRecord->AccountType, &newRecord.AccountType, sizeof(liveRecord->AccountType));
		std::memcpy(&liveRecord->ProductClass, &newRecord.ProductClass, sizeof(liveRecord->ProductClass));
		std::memcpy(&liveRecord->Volume, &newRecord.Volume, sizeof(liveRecord->Volume));
		std::memcpy(&liveRecord->OpenPrice, &newRecord.OpenPrice, sizeof(liveRecord->OpenPrice));
		std::memcpy(&liveRecord->MarketValue, &newRecord.MarketValue, sizeof(liveRecord->MarketValue));
		std::memcpy(&liveRecord->CashIn, &newRecord.CashIn, sizeof(liveRecord->CashIn));
		std::memcpy(&liveRecord->CashOut, &newRecord.CashOut, sizeof(liveRecord->CashOut));
		std::memcpy(&liveRecord->Margin, &newRecord.Margin, sizeof(liveRecord->Margin));
		std::memcpy(&liveRecord->Commission, &newRecord.Commission, sizeof(liveRecord->Commission));
		std::memcpy(&liveRecord->VolumeMultiple, &newRecord.VolumeMultiple, sizeof(liveRecord->VolumeMultiple));
		std::memcpy(&liveRecord->CloseProfitByDate, &newRecord.CloseProfitByDate, sizeof(liveRecord->CloseProfitByDate));
		std::memcpy(&liveRecord->CloseProfitByTrade, &newRecord.CloseProfitByTrade, sizeof(liveRecord->CloseProfitByTrade));
		std::memcpy(&liveRecord->PositionProfitByDate, &newRecord.PositionProfitByDate, sizeof(liveRecord->PositionProfitByDate));
		std::memcpy(&liveRecord->PositionProfitByTrade, &newRecord.PositionProfitByTrade, sizeof(liveRecord->PositionProfitByTrade));
		std::memcpy(&liveRecord->SettlementPrice, &newRecord.SettlementPrice, sizeof(liveRecord->SettlementPrice));
		std::memcpy(&liveRecord->PreSettlementPrice, &newRecord.PreSettlementPrice, sizeof(liveRecord->PreSettlementPrice));
		std::memcpy(&liveRecord->CloseVolume, &newRecord.CloseVolume, sizeof(liveRecord->CloseVolume));
		std::memcpy(&liveRecord->CloseAmount, &newRecord.CloseAmount, sizeof(liveRecord->CloseAmount));
		slot.Version.store(before + 2, std::memory_order_release);
	}

	void PositionDetailTablePrimaryKey::Clear()
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		root_.store(CreateSlotArray(InitialSlotCapacity), std::memory_order_release);
		rowCount_ = 0;
		usedSlotCount_ = 0;
		epoch_.Retire(previousSlotArray, ReclaimSlotArrayWithRecords);
		epoch_.AdvanceAndReclaim();
	}

	const PositionDetailSlot* PositionDetailTablePrimaryKey::FindLiveSlot(const PositionDetail* keyRecord) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		std::size_t position = HashKey(keyRecord) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const PositionDetailSlot* slot = &slotArray->Slots[position];
			const PositionDetail* candidate = slot->Record.load(std::memory_order_acquire);
			if (candidate == nullptr)
			{
				return nullptr;
			}
			if (IsLiveSlot(candidate) && PositionDetailEqualForPositionDetailPrimaryKey()(candidate, keyRecord))
			{
				return slot;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return nullptr;
	}

	std::size_t PositionDetailTablePrimaryKey::FindOccupiedSlotPosition(const PositionDetail* record) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		std::size_t position = HashKey(record) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const PositionDetail* candidate = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (candidate == nullptr)
			{
				break;
			}
			if (candidate == record)
			{
				return position;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return slotArray->Capacity;
	}

	std::size_t PositionDetailTablePrimaryKey::FindFreeSlotPosition(const PositionDetail* keyRecord) const
	{
		return FindFreeSlotPositionIn(*root_.load(std::memory_order_relaxed), keyRecord);
	}

	std::size_t PositionDetailTablePrimaryKey::FindFreeSlotPositionIn(const SlotArray& slotArray, const PositionDetail* keyRecord)
	{
		std::size_t position = HashKey(keyRecord) % slotArray.Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray.Capacity; ++probedSlotCount)
		{
			if (!IsLiveSlot(slotArray.Slots[position].Record.load(std::memory_order_relaxed)))
			{
				return position;
			}
			position = (position + 1) % slotArray.Capacity;
		}
		return slotArray.Capacity;
	}

	void PositionDetailTablePrimaryKey::RebuildForInsertIfNeeded()
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		if ((usedSlotCount_ + 1) * 4 <= slotArray->Capacity * 3)
		{
			return;
		}
		Rebuild(slotArray->Capacity * 2);
	}

	void PositionDetailTablePrimaryKey::Rebuild(std::size_t capacity)
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		SlotArray* slotArray = CreateSlotArray(capacity);
		ForEachRecord([slotArray](PositionDetail* record)
		{
			const std::size_t position = FindFreeSlotPositionIn(*slotArray, record);
			slotArray->Slots[position].Record.store(record, std::memory_order_relaxed);
			return true;
		});
		root_.store(slotArray, std::memory_order_release);
		usedSlotCount_ = rowCount_;
		// 记录已搬进新槽数组，旧数组只回收槽位本身
		epoch_.Retire(previousSlotArray, ReclaimSlotArray);
		epoch_.AdvanceAndReclaim();
	}

	PositionDetailTablePrimaryKey::SlotArray* PositionDetailTablePrimaryKey::CreateSlotArray(std::size_t capacity)
	{
		return new SlotArray{ new PositionDetailSlot[capacity]{}, capacity };
	}

	void PositionDetailTablePrimaryKey::ReclaimSlotArray(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void PositionDetailTablePrimaryKey::ReclaimSlotArrayWithRecords(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		for (std::size_t position = 0; position < slotArray->Capacity; ++position)
		{
			PositionDetail* record = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (IsLiveSlot(record))
			{
				record->Deallocate();
			}
		}
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void PositionDetailTablePrimaryKey::ReclaimRecord(void* object) noexcept
	{
		static_cast<PositionDetail*>(object)->Deallocate();
	}

	std::size_t OrderTablePrimaryKey::HashKey(const Order* record) noexcept
	{
		return OrderHashForOrderPrimaryKey()(record);
	}

	Order* OrderTablePrimaryKey::TombstoneSlot() noexcept
	{
		return reinterpret_cast<Order*>(static_cast<std::uintptr_t>(1));
	}

	bool OrderTablePrimaryKey::IsLiveSlot(const Order* record) noexcept
	{
		return record != nullptr && record != TombstoneSlot();
	}

	OrderTablePrimaryKey::iterator::iterator(const OrderSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept
		: slotBegin_(slotBegin), slotCount_(slotCount), position_(position)
	{
		SkipUnoccupiedSlots();
	}

	Order* OrderTablePrimaryKey::iterator::operator*() const noexcept
	{
		return slotBegin_[position_].Record.load(std::memory_order_acquire);
	}

	OrderTablePrimaryKey::iterator& OrderTablePrimaryKey::iterator::operator++() noexcept
	{
		++position_;
		SkipUnoccupiedSlots();
		return *this;
	}

	OrderTablePrimaryKey::iterator OrderTablePrimaryKey::iterator::operator++(int) noexcept
	{
		iterator previous = *this;
		++(*this);
		return previous;
	}

	bool OrderTablePrimaryKey::iterator::operator==(const iterator& other) const noexcept
	{
		return slotBegin_ == other.slotBegin_ && position_ == other.position_;
	}

	bool OrderTablePrimaryKey::iterator::operator!=(const iterator& other) const noexcept
	{
		return !(*this == other);
	}

	void OrderTablePrimaryKey::iterator::SkipUnoccupiedSlots() noexcept
	{
		while (position_ < slotCount_ && !IsLiveSlot(slotBegin_[position_].Record.load(std::memory_order_relaxed)))
		{
			++position_;
		}
	}

	OrderTablePrimaryKey::OrderTablePrimaryKey(OrderTable* tableOwner)
		: table_(tableOwner), root_(CreateSlotArray(InitialSlotCapacity))
	{
	}

	OrderTablePrimaryKey::~OrderTablePrimaryKey()
	{
		// 表已不可达，活记录与退休对象一次性归还，不必再等读者
		ReclaimSlotArrayWithRecords(root_.load(std::memory_order_acquire));
		root_.store(nullptr, std::memory_order_relaxed);
		epoch_.ReclaimAllWithoutReaders();
	}

	std::optional<Order> OrderTablePrimaryKey::Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const OrderIdType& OrderId)
	{
		// 登记代数后按 seqlock 拷贝：写者在改同一行时版本为奇数，读者重来
		Rcu::ReaderEpoch::Guard guard(epoch_, Rcu::CurrentReaderIndex());
		Order keyRecord{};
		Utility::Strcpy(keyRecord.TradingDay, TradingDay);
		Utility::Strcpy(keyRecord.AccountId, AccountId);
		Utility::Strcpy(keyRecord.ExchangeId, ExchangeId);
		Utility::Strcpy(keyRecord.InstrumentId, InstrumentId);
		keyRecord.OrderId = OrderId;
		const OrderSlot* slot = FindLiveSlot(&keyRecord);
		if (slot == nullptr)
		{
			return std::nullopt;
		}
		for (;;)
		{
			const std::uint64_t before = slot->Version.load(std::memory_order_acquire);
			if ((before & 1) != 0)
			{
				std::this_thread::yield();
				continue;
			}
			const Order* liveRecord = slot->Record.load(std::memory_order_acquire);
			if (!IsLiveSlot(liveRecord))
			{
				return std::nullopt;
			}
			Order copy{};
			std::memcpy(&copy, liveRecord, sizeof(Order));
			std::atomic_thread_fence(std::memory_order_acquire);
			if (slot->Version.load(std::memory_order_relaxed) == before)
			{
				return copy;
			}
			std::this_thread::yield();
		}
	}

	TableRangeLease<OrderTablePrimaryKey::iterator> OrderTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		const iterator first(slotArray->Slots, slotArray->Capacity, 0);
		const iterator last(slotArray->Slots, slotArray->Capacity, slotArray->Capacity);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}

	bool OrderTablePrimaryKey::Insert(Order* const record)
	{
		RebuildForInsertIfNeeded();
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindFreeSlotPosition(record);
		if (slotArray->Slots[position].Record.load(std::memory_order_relaxed) == nullptr)
		{
			++usedSlotCount_;
		}
		slotArray->Slots[position].Record.store(record, std::memory_order_release);
		++rowCount_;
		return true;
	}

	void OrderTablePrimaryKey::Erase(Order* const record)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(record);
		assert(position < slotArray->Capacity);
		slotArray->Slots[position].Record.store(TombstoneSlot(), std::memory_order_release);
		--rowCount_;
		epoch_.Retire(record, ReclaimRecord);
		epoch_.AdvanceAndReclaim();
	}

	bool OrderTablePrimaryKey::CheckInsert(Order* const record)
	{
		return FindLiveSlot(record) == nullptr;
	}

	Order* OrderTablePrimaryKey::Find(const Order* record)
	{
		const OrderSlot* slot = FindLiveSlot(record);
		return slot == nullptr ? nullptr : const_cast<Order*>(slot->Record.load(std::memory_order_relaxed));
	}

	void OrderTablePrimaryKey::Overwrite(Order* const liveRecord, const Order& newRecord)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(liveRecord);
		assert(position < slotArray->Capacity);
		OrderSlot& slot = slotArray->Slots[position];
		const std::uint64_t before = slot.Version.load(std::memory_order_relaxed);
		slot.Version.store(before + 1, std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_release);
		// 主键字段不可变，不重写：避免与无锁查找读主键竞争
		std::memcpy(&liveRecord->AccountType, &newRecord.AccountType, sizeof(liveRecord->AccountType));
		std::memcpy(&liveRecord->ProductClass, &newRecord.ProductClass, sizeof(liveRecord->ProductClass));
		std::memcpy(&liveRecord->OrderSysId, &newRecord.OrderSysId, sizeof(liveRecord->OrderSysId));
		std::memcpy(&liveRecord->Direction, &newRecord.Direction, sizeof(liveRecord->Direction));
		std::memcpy(&liveRecord->OffsetFlag, &newRecord.OffsetFlag, sizeof(liveRecord->OffsetFlag));
		std::memcpy(&liveRecord->OrderPriceType, &newRecord.OrderPriceType, sizeof(liveRecord->OrderPriceType));
		std::memcpy(&liveRecord->Price, &newRecord.Price, sizeof(liveRecord->Price));
		std::memcpy(&liveRecord->Volume, &newRecord.Volume, sizeof(liveRecord->Volume));
		std::memcpy(&liveRecord->VolumeTotal, &newRecord.VolumeTotal, sizeof(liveRecord->VolumeTotal));
		std::memcpy(&liveRecord->VolumeTraded, &newRecord.VolumeTraded, sizeof(liveRecord->VolumeTraded));
		std::memcpy(&liveRecord->VolumeMultiple, &newRecord.VolumeMultiple, sizeof(liveRecord->VolumeMultiple));
		std::memcpy(&liveRecord->OrderStatus, &newRecord.OrderStatus, sizeof(liveRecord->OrderStatus));
		std::memcpy(&liveRecord->OrderDate, &newRecord.OrderDate, sizeof(liveRecord->OrderDate));
		std::memcpy(&liveRecord->OrderTime, &newRecord.OrderTime, sizeof(liveRecord->OrderTime));
		std::memcpy(&liveRecord->CancelDate, &newRecord.CancelDate, sizeof(liveRecord->CancelDate));
		std::memcpy(&liveRecord->CancelTime, &newRecord.CancelTime, sizeof(liveRecord->CancelTime));
		std::memcpy(&liveRecord->SessionId, &newRecord.SessionId, sizeof(liveRecord->SessionId));
		std::memcpy(&liveRecord->ClientOrderId, &newRecord.ClientOrderId, sizeof(liveRecord->ClientOrderId));
		std::memcpy(&liveRecord->RequestId, &newRecord.RequestId, sizeof(liveRecord->RequestId));
		std::memcpy(&liveRecord->OfferId, &newRecord.OfferId, sizeof(liveRecord->OfferId));
		std::memcpy(&liveRecord->TradeGroupId, &newRecord.TradeGroupId, sizeof(liveRecord->TradeGroupId));
		std::memcpy(&liveRecord->RiskGroupId, &newRecord.RiskGroupId, sizeof(liveRecord->RiskGroupId));
		std::memcpy(&liveRecord->CommissionGroupId, &newRecord.CommissionGroupId, sizeof(liveRecord->CommissionGroupId));
		std::memcpy(&liveRecord->FrozenCash, &newRecord.FrozenCash, sizeof(liveRecord->FrozenCash));
		std::memcpy(&liveRecord->FrozenMargin, &newRecord.FrozenMargin, sizeof(liveRecord->FrozenMargin));
		std::memcpy(&liveRecord->FrozenCommission, &newRecord.FrozenCommission, sizeof(liveRecord->FrozenCommission));
		std::memcpy(&liveRecord->RebuildMark, &newRecord.RebuildMark, sizeof(liveRecord->RebuildMark));
		std::memcpy(&liveRecord->IsForceClose, &newRecord.IsForceClose, sizeof(liveRecord->IsForceClose));
		slot.Version.store(before + 2, std::memory_order_release);
	}

	void OrderTablePrimaryKey::Clear()
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		root_.store(CreateSlotArray(InitialSlotCapacity), std::memory_order_release);
		rowCount_ = 0;
		usedSlotCount_ = 0;
		epoch_.Retire(previousSlotArray, ReclaimSlotArrayWithRecords);
		epoch_.AdvanceAndReclaim();
	}

	const OrderSlot* OrderTablePrimaryKey::FindLiveSlot(const Order* keyRecord) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		std::size_t position = HashKey(keyRecord) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const OrderSlot* slot = &slotArray->Slots[position];
			const Order* candidate = slot->Record.load(std::memory_order_acquire);
			if (candidate == nullptr)
			{
				return nullptr;
			}
			if (IsLiveSlot(candidate) && OrderEqualForOrderPrimaryKey()(candidate, keyRecord))
			{
				return slot;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return nullptr;
	}

	std::size_t OrderTablePrimaryKey::FindOccupiedSlotPosition(const Order* record) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		std::size_t position = HashKey(record) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const Order* candidate = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (candidate == nullptr)
			{
				break;
			}
			if (candidate == record)
			{
				return position;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return slotArray->Capacity;
	}

	std::size_t OrderTablePrimaryKey::FindFreeSlotPosition(const Order* keyRecord) const
	{
		return FindFreeSlotPositionIn(*root_.load(std::memory_order_relaxed), keyRecord);
	}

	std::size_t OrderTablePrimaryKey::FindFreeSlotPositionIn(const SlotArray& slotArray, const Order* keyRecord)
	{
		std::size_t position = HashKey(keyRecord) % slotArray.Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray.Capacity; ++probedSlotCount)
		{
			if (!IsLiveSlot(slotArray.Slots[position].Record.load(std::memory_order_relaxed)))
			{
				return position;
			}
			position = (position + 1) % slotArray.Capacity;
		}
		return slotArray.Capacity;
	}

	void OrderTablePrimaryKey::RebuildForInsertIfNeeded()
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		if ((usedSlotCount_ + 1) * 4 <= slotArray->Capacity * 3)
		{
			return;
		}
		Rebuild(slotArray->Capacity * 2);
	}

	void OrderTablePrimaryKey::Rebuild(std::size_t capacity)
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		SlotArray* slotArray = CreateSlotArray(capacity);
		ForEachRecord([slotArray](Order* record)
		{
			const std::size_t position = FindFreeSlotPositionIn(*slotArray, record);
			slotArray->Slots[position].Record.store(record, std::memory_order_relaxed);
			return true;
		});
		root_.store(slotArray, std::memory_order_release);
		usedSlotCount_ = rowCount_;
		// 记录已搬进新槽数组，旧数组只回收槽位本身
		epoch_.Retire(previousSlotArray, ReclaimSlotArray);
		epoch_.AdvanceAndReclaim();
	}

	OrderTablePrimaryKey::SlotArray* OrderTablePrimaryKey::CreateSlotArray(std::size_t capacity)
	{
		return new SlotArray{ new OrderSlot[capacity]{}, capacity };
	}

	void OrderTablePrimaryKey::ReclaimSlotArray(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void OrderTablePrimaryKey::ReclaimSlotArrayWithRecords(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		for (std::size_t position = 0; position < slotArray->Capacity; ++position)
		{
			Order* record = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (IsLiveSlot(record))
			{
				record->Deallocate();
			}
		}
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void OrderTablePrimaryKey::ReclaimRecord(void* object) noexcept
	{
		static_cast<Order*>(object)->Deallocate();
	}

	OrderTableUniqueKeyClientOrderId::OrderTableUniqueKeyClientOrderId(OrderTable* tableOwner)
		: table_(tableOwner)
	{
	}

	std::optional<Order> OrderTableUniqueKeyClientOrderId::Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const SessionIdType& SessionId, const ClientOrderIdType& ClientOrderId)
	{
		Order keyRecord{};
		Utility::Strcpy(keyRecord.TradingDay, TradingDay);
		Utility::Strcpy(keyRecord.AccountId, AccountId);
		Utility::Strcpy(keyRecord.ExchangeId, ExchangeId);
		Utility::Strcpy(keyRecord.InstrumentId, InstrumentId);
		keyRecord.SessionId = SessionId;
		keyRecord.ClientOrderId = ClientOrderId;
		// 写者改表时持 SharedMutex 独占锁，共享锁内取到的指针不会被改写或回收
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&keyRecord);
		if (it == index_.end())
		{
			return std::nullopt;
		}
		return **it;
	}

	bool OrderTableUniqueKeyClientOrderId::Insert(Order* const record)
	{
		return index_.insert(record).second;
	}

	void OrderTableUniqueKeyClientOrderId::Erase(Order* const record)
	{
		index_.erase(record);
	}

	bool OrderTableUniqueKeyClientOrderId::CheckInsert(Order* const record)
	{
		return index_.find(record) == index_.end();
	}

	bool OrderTableUniqueKeyClientOrderId::CheckUpdate(const Order* const oldRecord, const Order* const newRecord)
	{
		return OrderEqualForClientOrderIdUniqueKey()(oldRecord, newRecord);
	}

	std::size_t TradeTablePrimaryKey::HashKey(const Trade* record) noexcept
	{
		return TradeHashForTradePrimaryKey()(record);
	}

	Trade* TradeTablePrimaryKey::TombstoneSlot() noexcept
	{
		return reinterpret_cast<Trade*>(static_cast<std::uintptr_t>(1));
	}

	bool TradeTablePrimaryKey::IsLiveSlot(const Trade* record) noexcept
	{
		return record != nullptr && record != TombstoneSlot();
	}

	TradeTablePrimaryKey::iterator::iterator(const TradeSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept
		: slotBegin_(slotBegin), slotCount_(slotCount), position_(position)
	{
		SkipUnoccupiedSlots();
	}

	Trade* TradeTablePrimaryKey::iterator::operator*() const noexcept
	{
		return slotBegin_[position_].Record.load(std::memory_order_acquire);
	}

	TradeTablePrimaryKey::iterator& TradeTablePrimaryKey::iterator::operator++() noexcept
	{
		++position_;
		SkipUnoccupiedSlots();
		return *this;
	}

	TradeTablePrimaryKey::iterator TradeTablePrimaryKey::iterator::operator++(int) noexcept
	{
		iterator previous = *this;
		++(*this);
		return previous;
	}

	bool TradeTablePrimaryKey::iterator::operator==(const iterator& other) const noexcept
	{
		return slotBegin_ == other.slotBegin_ && position_ == other.position_;
	}

	bool TradeTablePrimaryKey::iterator::operator!=(const iterator& other) const noexcept
	{
		return !(*this == other);
	}

	void TradeTablePrimaryKey::iterator::SkipUnoccupiedSlots() noexcept
	{
		while (position_ < slotCount_ && !IsLiveSlot(slotBegin_[position_].Record.load(std::memory_order_relaxed)))
		{
			++position_;
		}
	}

	TradeTablePrimaryKey::TradeTablePrimaryKey(TradeTable* tableOwner)
		: table_(tableOwner), root_(CreateSlotArray(InitialSlotCapacity))
	{
	}

	TradeTablePrimaryKey::~TradeTablePrimaryKey()
	{
		// 表已不可达，活记录与退休对象一次性归还，不必再等读者
		ReclaimSlotArrayWithRecords(root_.load(std::memory_order_acquire));
		root_.store(nullptr, std::memory_order_relaxed);
		epoch_.ReclaimAllWithoutReaders();
	}

	std::optional<Trade> TradeTablePrimaryKey::Select(const DateType& TradingDay, const ExchangeIdType& ExchangeId, const TradeIdType& TradeId, const DirectionType& Direction)
	{
		// 登记代数后按 seqlock 拷贝：写者在改同一行时版本为奇数，读者重来
		Rcu::ReaderEpoch::Guard guard(epoch_, Rcu::CurrentReaderIndex());
		Trade keyRecord{};
		Utility::Strcpy(keyRecord.TradingDay, TradingDay);
		Utility::Strcpy(keyRecord.ExchangeId, ExchangeId);
		Utility::Strcpy(keyRecord.TradeId, TradeId);
		keyRecord.Direction = Direction;
		const TradeSlot* slot = FindLiveSlot(&keyRecord);
		if (slot == nullptr)
		{
			return std::nullopt;
		}
		for (;;)
		{
			const std::uint64_t before = slot->Version.load(std::memory_order_acquire);
			if ((before & 1) != 0)
			{
				std::this_thread::yield();
				continue;
			}
			const Trade* liveRecord = slot->Record.load(std::memory_order_acquire);
			if (!IsLiveSlot(liveRecord))
			{
				return std::nullopt;
			}
			Trade copy{};
			std::memcpy(&copy, liveRecord, sizeof(Trade));
			std::atomic_thread_fence(std::memory_order_acquire);
			if (slot->Version.load(std::memory_order_relaxed) == before)
			{
				return copy;
			}
			std::this_thread::yield();
		}
	}

	TableRangeLease<TradeTablePrimaryKey::iterator> TradeTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		const iterator first(slotArray->Slots, slotArray->Capacity, 0);
		const iterator last(slotArray->Slots, slotArray->Capacity, slotArray->Capacity);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}

	bool TradeTablePrimaryKey::Insert(Trade* const record)
	{
		RebuildForInsertIfNeeded();
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindFreeSlotPosition(record);
		if (slotArray->Slots[position].Record.load(std::memory_order_relaxed) == nullptr)
		{
			++usedSlotCount_;
		}
		slotArray->Slots[position].Record.store(record, std::memory_order_release);
		++rowCount_;
		return true;
	}

	void TradeTablePrimaryKey::Erase(Trade* const record)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(record);
		assert(position < slotArray->Capacity);
		slotArray->Slots[position].Record.store(TombstoneSlot(), std::memory_order_release);
		--rowCount_;
		epoch_.Retire(record, ReclaimRecord);
		epoch_.AdvanceAndReclaim();
	}

	bool TradeTablePrimaryKey::CheckInsert(Trade* const record)
	{
		return FindLiveSlot(record) == nullptr;
	}

	Trade* TradeTablePrimaryKey::Find(const Trade* record)
	{
		const TradeSlot* slot = FindLiveSlot(record);
		return slot == nullptr ? nullptr : const_cast<Trade*>(slot->Record.load(std::memory_order_relaxed));
	}

	void TradeTablePrimaryKey::Overwrite(Trade* const liveRecord, const Trade& newRecord)
	{
		SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		const std::size_t position = FindOccupiedSlotPosition(liveRecord);
		assert(position < slotArray->Capacity);
		TradeSlot& slot = slotArray->Slots[position];
		const std::uint64_t before = slot.Version.load(std::memory_order_relaxed);
		slot.Version.store(before + 1, std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_release);
		// 主键字段不可变，不重写：避免与无锁查找读主键竞争
		std::memcpy(&liveRecord->AccountId, &newRecord.AccountId, sizeof(liveRecord->AccountId));
		std::memcpy(&liveRecord->AccountType, &newRecord.AccountType, sizeof(liveRecord->AccountType));
		std::memcpy(&liveRecord->InstrumentId, &newRecord.InstrumentId, sizeof(liveRecord->InstrumentId));
		std::memcpy(&liveRecord->ProductClass, &newRecord.ProductClass, sizeof(liveRecord->ProductClass));
		std::memcpy(&liveRecord->OrderId, &newRecord.OrderId, sizeof(liveRecord->OrderId));
		std::memcpy(&liveRecord->OrderSysId, &newRecord.OrderSysId, sizeof(liveRecord->OrderSysId));
		std::memcpy(&liveRecord->OffsetFlag, &newRecord.OffsetFlag, sizeof(liveRecord->OffsetFlag));
		std::memcpy(&liveRecord->Price, &newRecord.Price, sizeof(liveRecord->Price));
		std::memcpy(&liveRecord->Volume, &newRecord.Volume, sizeof(liveRecord->Volume));
		std::memcpy(&liveRecord->VolumeMultiple, &newRecord.VolumeMultiple, sizeof(liveRecord->VolumeMultiple));
		std::memcpy(&liveRecord->TradeAmount, &newRecord.TradeAmount, sizeof(liveRecord->TradeAmount));
		std::memcpy(&liveRecord->Commission, &newRecord.Commission, sizeof(liveRecord->Commission));
		std::memcpy(&liveRecord->TradeDate, &newRecord.TradeDate, sizeof(liveRecord->TradeDate));
		std::memcpy(&liveRecord->TradeTime, &newRecord.TradeTime, sizeof(liveRecord->TradeTime));
		slot.Version.store(before + 2, std::memory_order_release);
	}

	void TradeTablePrimaryKey::Clear()
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		root_.store(CreateSlotArray(InitialSlotCapacity), std::memory_order_release);
		rowCount_ = 0;
		usedSlotCount_ = 0;
		epoch_.Retire(previousSlotArray, ReclaimSlotArrayWithRecords);
		epoch_.AdvanceAndReclaim();
	}

	const TradeSlot* TradeTablePrimaryKey::FindLiveSlot(const Trade* keyRecord) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_acquire);
		std::size_t position = HashKey(keyRecord) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const TradeSlot* slot = &slotArray->Slots[position];
			const Trade* candidate = slot->Record.load(std::memory_order_acquire);
			if (candidate == nullptr)
			{
				return nullptr;
			}
			if (IsLiveSlot(candidate) && TradeEqualForTradePrimaryKey()(candidate, keyRecord))
			{
				return slot;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return nullptr;
	}

	std::size_t TradeTablePrimaryKey::FindOccupiedSlotPosition(const Trade* record) const
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		std::size_t position = HashKey(record) % slotArray->Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray->Capacity; ++probedSlotCount)
		{
			const Trade* candidate = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (candidate == nullptr)
			{
				break;
			}
			if (candidate == record)
			{
				return position;
			}
			position = (position + 1) % slotArray->Capacity;
		}
		return slotArray->Capacity;
	}

	std::size_t TradeTablePrimaryKey::FindFreeSlotPosition(const Trade* keyRecord) const
	{
		return FindFreeSlotPositionIn(*root_.load(std::memory_order_relaxed), keyRecord);
	}

	std::size_t TradeTablePrimaryKey::FindFreeSlotPositionIn(const SlotArray& slotArray, const Trade* keyRecord)
	{
		std::size_t position = HashKey(keyRecord) % slotArray.Capacity;
		for (std::size_t probedSlotCount = 0; probedSlotCount < slotArray.Capacity; ++probedSlotCount)
		{
			if (!IsLiveSlot(slotArray.Slots[position].Record.load(std::memory_order_relaxed)))
			{
				return position;
			}
			position = (position + 1) % slotArray.Capacity;
		}
		return slotArray.Capacity;
	}

	void TradeTablePrimaryKey::RebuildForInsertIfNeeded()
	{
		const SlotArray* slotArray = root_.load(std::memory_order_relaxed);
		if ((usedSlotCount_ + 1) * 4 <= slotArray->Capacity * 3)
		{
			return;
		}
		Rebuild(slotArray->Capacity * 2);
	}

	void TradeTablePrimaryKey::Rebuild(std::size_t capacity)
	{
		SlotArray* previousSlotArray = root_.load(std::memory_order_relaxed);
		SlotArray* slotArray = CreateSlotArray(capacity);
		ForEachRecord([slotArray](Trade* record)
		{
			const std::size_t position = FindFreeSlotPositionIn(*slotArray, record);
			slotArray->Slots[position].Record.store(record, std::memory_order_relaxed);
			return true;
		});
		root_.store(slotArray, std::memory_order_release);
		usedSlotCount_ = rowCount_;
		// 记录已搬进新槽数组，旧数组只回收槽位本身
		epoch_.Retire(previousSlotArray, ReclaimSlotArray);
		epoch_.AdvanceAndReclaim();
	}

	TradeTablePrimaryKey::SlotArray* TradeTablePrimaryKey::CreateSlotArray(std::size_t capacity)
	{
		return new SlotArray{ new TradeSlot[capacity]{}, capacity };
	}

	void TradeTablePrimaryKey::ReclaimSlotArray(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void TradeTablePrimaryKey::ReclaimSlotArrayWithRecords(void* object) noexcept
	{
		SlotArray* slotArray = static_cast<SlotArray*>(object);
		for (std::size_t position = 0; position < slotArray->Capacity; ++position)
		{
			Trade* record = slotArray->Slots[position].Record.load(std::memory_order_relaxed);
			if (IsLiveSlot(record))
			{
				record->Deallocate();
			}
		}
		delete[] slotArray->Slots;
		delete slotArray;
	}

	void TradeTablePrimaryKey::ReclaimRecord(void* object) noexcept
	{
		static_cast<Trade*>(object)->Deallocate();
	}

}

