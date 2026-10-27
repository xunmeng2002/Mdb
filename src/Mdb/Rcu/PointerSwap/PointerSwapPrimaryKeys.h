// 本文件由 ../Templates/Cpp/Mdb/Rcu/PointerSwap/PointerSwapPrimaryKeys.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "Model/MdbStructs.h"
#include "Model/MdbReadLease.h"
#include "Model/MdbPrimaryKeyComp.h"
#include "Rcu/RcuEpoch.h"
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <optional>

namespace Mdb::Rcu::PointerSwap
{
	struct TradingDaySlot
	{
		std::atomic<TradingDay*> Record{ nullptr };
	};

	class TradingDayTable;

	// 主键索引：开放寻址的原子槽位数组，槽数组由根指针原子发布。
	// Select 无锁读拷贝；SelectAll 沿用 Mdb 的表级读租约，期间写者被挡在 SharedMutex 外。
	class TradingDayTablePrimaryKey
	{
	public:
		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = TradingDay*;
			using difference_type = std::ptrdiff_t;
			using pointer = TradingDay* const*;
			using reference = TradingDay*;

			iterator() noexcept = default;
			TradingDay* operator*() const noexcept;
			iterator& operator++() noexcept;
			iterator operator++(int) noexcept;
			bool operator==(const iterator& other) const noexcept;
			bool operator!=(const iterator& other) const noexcept;

		private:
			friend class TradingDayTablePrimaryKey;

			iterator(const TradingDaySlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept;
			void SkipUnoccupiedSlots() noexcept;

			const TradingDaySlot* slotBegin_ = nullptr;
			std::size_t slotCount_ = 0;
			std::size_t position_ = 0;
		};

		explicit TradingDayTablePrimaryKey(TradingDayTable* tableOwner);
		~TradingDayTablePrimaryKey();
		TradingDayTablePrimaryKey(const TradingDayTablePrimaryKey&) = delete;
		TradingDayTablePrimaryKey& operator=(const TradingDayTablePrimaryKey&) = delete;

		std::optional<TradingDay> Select(const Int32Type& Pk);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(TradingDay* const record);
		void Erase(TradingDay* const record);
		bool CheckInsert(TradingDay* const record);
		// 调用方必须已持有 SharedMutex 独占锁；返回表内活记录，不存在则为空
		TradingDay* Find(const TradingDay* record);
		// 调用方必须已持有 SharedMutex 独占锁：新行换掉旧行，旧行退休
		void Replace(TradingDay* const liveRecord, const TradingDay& newRecord);
		// 调用方必须已持有 SharedMutex 独占锁：整表清空，旧槽数组连同活记录一起退休
		void Clear();

		template <typename Visitor>
		void ForEachRecord(Visitor&& visitor) const
		{
			const SlotArray* slotArray = root_.load(std::memory_order_acquire);
			for (std::size_t position = 0; position < slotArray->Capacity; ++position)
			{
				TradingDay* record = slotArray->Slots[position].Record.load(std::memory_order_acquire);
				if (record != nullptr && record != TombstoneSlot() && !visitor(record))
				{
					return;
				}
			}
		}

	private:
		friend class TradingDayTable;

		struct SlotArray
		{
			TradingDaySlot* Slots;
			std::size_t Capacity;
		};

		static constexpr std::size_t InitialSlotCapacity = 1024;

		static std::size_t HashKey(const TradingDay* record) noexcept;
		static TradingDay* TombstoneSlot() noexcept;
		static bool IsLiveSlot(const TradingDay* record) noexcept;
		static SlotArray* CreateSlotArray(std::size_t capacity);
		static std::size_t FindFreeSlotPositionIn(const SlotArray& slotArray, const TradingDay* keyRecord);
		static void ReclaimSlotArray(void* object) noexcept;
		static void ReclaimSlotArrayWithRecords(void* object) noexcept;
		static void ReclaimRecord(void* object) noexcept;
		const TradingDay* FindLiveRecord(const TradingDay* keyRecord) const;
		std::size_t FindOccupiedSlotPosition(const TradingDay* record) const;
		std::size_t FindFreeSlotPosition(const TradingDay* keyRecord) const;
		void RebuildForInsertIfNeeded();
		void Rebuild(std::size_t capacity);

		TradingDayTable* table_;
		std::atomic<SlotArray*> root_;
		std::size_t rowCount_ = 0;
		std::size_t usedSlotCount_ = 0;
		Rcu::ReaderEpoch epoch_;
	};

	struct ExchangeSlot
	{
		std::atomic<Exchange*> Record{ nullptr };
	};

	class ExchangeTable;

	// 主键索引：开放寻址的原子槽位数组，槽数组由根指针原子发布。
	// Select 无锁读拷贝；SelectAll 沿用 Mdb 的表级读租约，期间写者被挡在 SharedMutex 外。
	class ExchangeTablePrimaryKey
	{
	public:
		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = Exchange*;
			using difference_type = std::ptrdiff_t;
			using pointer = Exchange* const*;
			using reference = Exchange*;

			iterator() noexcept = default;
			Exchange* operator*() const noexcept;
			iterator& operator++() noexcept;
			iterator operator++(int) noexcept;
			bool operator==(const iterator& other) const noexcept;
			bool operator!=(const iterator& other) const noexcept;

		private:
			friend class ExchangeTablePrimaryKey;

			iterator(const ExchangeSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept;
			void SkipUnoccupiedSlots() noexcept;

			const ExchangeSlot* slotBegin_ = nullptr;
			std::size_t slotCount_ = 0;
			std::size_t position_ = 0;
		};

		explicit ExchangeTablePrimaryKey(ExchangeTable* tableOwner);
		~ExchangeTablePrimaryKey();
		ExchangeTablePrimaryKey(const ExchangeTablePrimaryKey&) = delete;
		ExchangeTablePrimaryKey& operator=(const ExchangeTablePrimaryKey&) = delete;

		std::optional<Exchange> Select(const ExchangeIdType& ExchangeId);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(Exchange* const record);
		void Erase(Exchange* const record);
		bool CheckInsert(Exchange* const record);
		// 调用方必须已持有 SharedMutex 独占锁；返回表内活记录，不存在则为空
		Exchange* Find(const Exchange* record);
		// 调用方必须已持有 SharedMutex 独占锁：新行换掉旧行，旧行退休
		void Replace(Exchange* const liveRecord, const Exchange& newRecord);
		// 调用方必须已持有 SharedMutex 独占锁：整表清空，旧槽数组连同活记录一起退休
		void Clear();

		template <typename Visitor>
		void ForEachRecord(Visitor&& visitor) const
		{
			const SlotArray* slotArray = root_.load(std::memory_order_acquire);
			for (std::size_t position = 0; position < slotArray->Capacity; ++position)
			{
				Exchange* record = slotArray->Slots[position].Record.load(std::memory_order_acquire);
				if (record != nullptr && record != TombstoneSlot() && !visitor(record))
				{
					return;
				}
			}
		}

	private:
		friend class ExchangeTable;

		struct SlotArray
		{
			ExchangeSlot* Slots;
			std::size_t Capacity;
		};

		static constexpr std::size_t InitialSlotCapacity = 1024;

		static std::size_t HashKey(const Exchange* record) noexcept;
		static Exchange* TombstoneSlot() noexcept;
		static bool IsLiveSlot(const Exchange* record) noexcept;
		static SlotArray* CreateSlotArray(std::size_t capacity);
		static std::size_t FindFreeSlotPositionIn(const SlotArray& slotArray, const Exchange* keyRecord);
		static void ReclaimSlotArray(void* object) noexcept;
		static void ReclaimSlotArrayWithRecords(void* object) noexcept;
		static void ReclaimRecord(void* object) noexcept;
		const Exchange* FindLiveRecord(const Exchange* keyRecord) const;
		std::size_t FindOccupiedSlotPosition(const Exchange* record) const;
		std::size_t FindFreeSlotPosition(const Exchange* keyRecord) const;
		void RebuildForInsertIfNeeded();
		void Rebuild(std::size_t capacity);

		ExchangeTable* table_;
		std::atomic<SlotArray*> root_;
		std::size_t rowCount_ = 0;
		std::size_t usedSlotCount_ = 0;
		Rcu::ReaderEpoch epoch_;
	};

	struct ProductSlot
	{
		std::atomic<Product*> Record{ nullptr };
	};

	class ProductTable;

	// 主键索引：开放寻址的原子槽位数组，槽数组由根指针原子发布。
	// Select 无锁读拷贝；SelectAll 沿用 Mdb 的表级读租约，期间写者被挡在 SharedMutex 外。
	class ProductTablePrimaryKey
	{
	public:
		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = Product*;
			using difference_type = std::ptrdiff_t;
			using pointer = Product* const*;
			using reference = Product*;

			iterator() noexcept = default;
			Product* operator*() const noexcept;
			iterator& operator++() noexcept;
			iterator operator++(int) noexcept;
			bool operator==(const iterator& other) const noexcept;
			bool operator!=(const iterator& other) const noexcept;

		private:
			friend class ProductTablePrimaryKey;

			iterator(const ProductSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept;
			void SkipUnoccupiedSlots() noexcept;

			const ProductSlot* slotBegin_ = nullptr;
			std::size_t slotCount_ = 0;
			std::size_t position_ = 0;
		};

		explicit ProductTablePrimaryKey(ProductTable* tableOwner);
		~ProductTablePrimaryKey();
		ProductTablePrimaryKey(const ProductTablePrimaryKey&) = delete;
		ProductTablePrimaryKey& operator=(const ProductTablePrimaryKey&) = delete;

		std::optional<Product> Select(const ExchangeIdType& ExchangeId, const ProductIdType& ProductId);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(Product* const record);
		void Erase(Product* const record);
		bool CheckInsert(Product* const record);
		// 调用方必须已持有 SharedMutex 独占锁；返回表内活记录，不存在则为空
		Product* Find(const Product* record);
		// 调用方必须已持有 SharedMutex 独占锁：新行换掉旧行，旧行退休
		void Replace(Product* const liveRecord, const Product& newRecord);
		// 调用方必须已持有 SharedMutex 独占锁：整表清空，旧槽数组连同活记录一起退休
		void Clear();

		template <typename Visitor>
		void ForEachRecord(Visitor&& visitor) const
		{
			const SlotArray* slotArray = root_.load(std::memory_order_acquire);
			for (std::size_t position = 0; position < slotArray->Capacity; ++position)
			{
				Product* record = slotArray->Slots[position].Record.load(std::memory_order_acquire);
				if (record != nullptr && record != TombstoneSlot() && !visitor(record))
				{
					return;
				}
			}
		}

	private:
		friend class ProductTable;

		struct SlotArray
		{
			ProductSlot* Slots;
			std::size_t Capacity;
		};

		static constexpr std::size_t InitialSlotCapacity = 1024;

		static std::size_t HashKey(const Product* record) noexcept;
		static Product* TombstoneSlot() noexcept;
		static bool IsLiveSlot(const Product* record) noexcept;
		static SlotArray* CreateSlotArray(std::size_t capacity);
		static std::size_t FindFreeSlotPositionIn(const SlotArray& slotArray, const Product* keyRecord);
		static void ReclaimSlotArray(void* object) noexcept;
		static void ReclaimSlotArrayWithRecords(void* object) noexcept;
		static void ReclaimRecord(void* object) noexcept;
		const Product* FindLiveRecord(const Product* keyRecord) const;
		std::size_t FindOccupiedSlotPosition(const Product* record) const;
		std::size_t FindFreeSlotPosition(const Product* keyRecord) const;
		void RebuildForInsertIfNeeded();
		void Rebuild(std::size_t capacity);

		ProductTable* table_;
		std::atomic<SlotArray*> root_;
		std::size_t rowCount_ = 0;
		std::size_t usedSlotCount_ = 0;
		Rcu::ReaderEpoch epoch_;
	};

	struct InstrumentSlot
	{
		std::atomic<Instrument*> Record{ nullptr };
	};

	class InstrumentTable;

	// 主键索引：开放寻址的原子槽位数组，槽数组由根指针原子发布。
	// Select 无锁读拷贝；SelectAll 沿用 Mdb 的表级读租约，期间写者被挡在 SharedMutex 外。
	class InstrumentTablePrimaryKey
	{
	public:
		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = Instrument*;
			using difference_type = std::ptrdiff_t;
			using pointer = Instrument* const*;
			using reference = Instrument*;

			iterator() noexcept = default;
			Instrument* operator*() const noexcept;
			iterator& operator++() noexcept;
			iterator operator++(int) noexcept;
			bool operator==(const iterator& other) const noexcept;
			bool operator!=(const iterator& other) const noexcept;

		private:
			friend class InstrumentTablePrimaryKey;

			iterator(const InstrumentSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept;
			void SkipUnoccupiedSlots() noexcept;

			const InstrumentSlot* slotBegin_ = nullptr;
			std::size_t slotCount_ = 0;
			std::size_t position_ = 0;
		};

		explicit InstrumentTablePrimaryKey(InstrumentTable* tableOwner);
		~InstrumentTablePrimaryKey();
		InstrumentTablePrimaryKey(const InstrumentTablePrimaryKey&) = delete;
		InstrumentTablePrimaryKey& operator=(const InstrumentTablePrimaryKey&) = delete;

		std::optional<Instrument> Select(const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(Instrument* const record);
		void Erase(Instrument* const record);
		bool CheckInsert(Instrument* const record);
		// 调用方必须已持有 SharedMutex 独占锁；返回表内活记录，不存在则为空
		Instrument* Find(const Instrument* record);
		// 调用方必须已持有 SharedMutex 独占锁：新行换掉旧行，旧行退休
		void Replace(Instrument* const liveRecord, const Instrument& newRecord);
		// 调用方必须已持有 SharedMutex 独占锁：整表清空，旧槽数组连同活记录一起退休
		void Clear();

		template <typename Visitor>
		void ForEachRecord(Visitor&& visitor) const
		{
			const SlotArray* slotArray = root_.load(std::memory_order_acquire);
			for (std::size_t position = 0; position < slotArray->Capacity; ++position)
			{
				Instrument* record = slotArray->Slots[position].Record.load(std::memory_order_acquire);
				if (record != nullptr && record != TombstoneSlot() && !visitor(record))
				{
					return;
				}
			}
		}

	private:
		friend class InstrumentTable;

		struct SlotArray
		{
			InstrumentSlot* Slots;
			std::size_t Capacity;
		};

		static constexpr std::size_t InitialSlotCapacity = 1024;

		static std::size_t HashKey(const Instrument* record) noexcept;
		static Instrument* TombstoneSlot() noexcept;
		static bool IsLiveSlot(const Instrument* record) noexcept;
		static SlotArray* CreateSlotArray(std::size_t capacity);
		static std::size_t FindFreeSlotPositionIn(const SlotArray& slotArray, const Instrument* keyRecord);
		static void ReclaimSlotArray(void* object) noexcept;
		static void ReclaimSlotArrayWithRecords(void* object) noexcept;
		static void ReclaimRecord(void* object) noexcept;
		const Instrument* FindLiveRecord(const Instrument* keyRecord) const;
		std::size_t FindOccupiedSlotPosition(const Instrument* record) const;
		std::size_t FindFreeSlotPosition(const Instrument* keyRecord) const;
		void RebuildForInsertIfNeeded();
		void Rebuild(std::size_t capacity);

		InstrumentTable* table_;
		std::atomic<SlotArray*> root_;
		std::size_t rowCount_ = 0;
		std::size_t usedSlotCount_ = 0;
		Rcu::ReaderEpoch epoch_;
	};

	struct PrimaryAccountSlot
	{
		std::atomic<PrimaryAccount*> Record{ nullptr };
	};

	class PrimaryAccountTable;

	// 主键索引：开放寻址的原子槽位数组，槽数组由根指针原子发布。
	// Select 无锁读拷贝；SelectAll 沿用 Mdb 的表级读租约，期间写者被挡在 SharedMutex 外。
	class PrimaryAccountTablePrimaryKey
	{
	public:
		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = PrimaryAccount*;
			using difference_type = std::ptrdiff_t;
			using pointer = PrimaryAccount* const*;
			using reference = PrimaryAccount*;

			iterator() noexcept = default;
			PrimaryAccount* operator*() const noexcept;
			iterator& operator++() noexcept;
			iterator operator++(int) noexcept;
			bool operator==(const iterator& other) const noexcept;
			bool operator!=(const iterator& other) const noexcept;

		private:
			friend class PrimaryAccountTablePrimaryKey;

			iterator(const PrimaryAccountSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept;
			void SkipUnoccupiedSlots() noexcept;

			const PrimaryAccountSlot* slotBegin_ = nullptr;
			std::size_t slotCount_ = 0;
			std::size_t position_ = 0;
		};

		explicit PrimaryAccountTablePrimaryKey(PrimaryAccountTable* tableOwner);
		~PrimaryAccountTablePrimaryKey();
		PrimaryAccountTablePrimaryKey(const PrimaryAccountTablePrimaryKey&) = delete;
		PrimaryAccountTablePrimaryKey& operator=(const PrimaryAccountTablePrimaryKey&) = delete;

		std::optional<PrimaryAccount> Select(const AccountIdType& PrimaryAccountId);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(PrimaryAccount* const record);
		void Erase(PrimaryAccount* const record);
		bool CheckInsert(PrimaryAccount* const record);
		// 调用方必须已持有 SharedMutex 独占锁；返回表内活记录，不存在则为空
		PrimaryAccount* Find(const PrimaryAccount* record);
		// 调用方必须已持有 SharedMutex 独占锁：新行换掉旧行，旧行退休
		void Replace(PrimaryAccount* const liveRecord, const PrimaryAccount& newRecord);
		// 调用方必须已持有 SharedMutex 独占锁：整表清空，旧槽数组连同活记录一起退休
		void Clear();

		template <typename Visitor>
		void ForEachRecord(Visitor&& visitor) const
		{
			const SlotArray* slotArray = root_.load(std::memory_order_acquire);
			for (std::size_t position = 0; position < slotArray->Capacity; ++position)
			{
				PrimaryAccount* record = slotArray->Slots[position].Record.load(std::memory_order_acquire);
				if (record != nullptr && record != TombstoneSlot() && !visitor(record))
				{
					return;
				}
			}
		}

	private:
		friend class PrimaryAccountTable;

		struct SlotArray
		{
			PrimaryAccountSlot* Slots;
			std::size_t Capacity;
		};

		static constexpr std::size_t InitialSlotCapacity = 1024;

		static std::size_t HashKey(const PrimaryAccount* record) noexcept;
		static PrimaryAccount* TombstoneSlot() noexcept;
		static bool IsLiveSlot(const PrimaryAccount* record) noexcept;
		static SlotArray* CreateSlotArray(std::size_t capacity);
		static std::size_t FindFreeSlotPositionIn(const SlotArray& slotArray, const PrimaryAccount* keyRecord);
		static void ReclaimSlotArray(void* object) noexcept;
		static void ReclaimSlotArrayWithRecords(void* object) noexcept;
		static void ReclaimRecord(void* object) noexcept;
		const PrimaryAccount* FindLiveRecord(const PrimaryAccount* keyRecord) const;
		std::size_t FindOccupiedSlotPosition(const PrimaryAccount* record) const;
		std::size_t FindFreeSlotPosition(const PrimaryAccount* keyRecord) const;
		void RebuildForInsertIfNeeded();
		void Rebuild(std::size_t capacity);

		PrimaryAccountTable* table_;
		std::atomic<SlotArray*> root_;
		std::size_t rowCount_ = 0;
		std::size_t usedSlotCount_ = 0;
		Rcu::ReaderEpoch epoch_;
	};

	struct AccountSlot
	{
		std::atomic<Account*> Record{ nullptr };
	};

	class AccountTable;

	// 主键索引：开放寻址的原子槽位数组，槽数组由根指针原子发布。
	// Select 无锁读拷贝；SelectAll 沿用 Mdb 的表级读租约，期间写者被挡在 SharedMutex 外。
	class AccountTablePrimaryKey
	{
	public:
		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = Account*;
			using difference_type = std::ptrdiff_t;
			using pointer = Account* const*;
			using reference = Account*;

			iterator() noexcept = default;
			Account* operator*() const noexcept;
			iterator& operator++() noexcept;
			iterator operator++(int) noexcept;
			bool operator==(const iterator& other) const noexcept;
			bool operator!=(const iterator& other) const noexcept;

		private:
			friend class AccountTablePrimaryKey;

			iterator(const AccountSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept;
			void SkipUnoccupiedSlots() noexcept;

			const AccountSlot* slotBegin_ = nullptr;
			std::size_t slotCount_ = 0;
			std::size_t position_ = 0;
		};

		explicit AccountTablePrimaryKey(AccountTable* tableOwner);
		~AccountTablePrimaryKey();
		AccountTablePrimaryKey(const AccountTablePrimaryKey&) = delete;
		AccountTablePrimaryKey& operator=(const AccountTablePrimaryKey&) = delete;

		std::optional<Account> Select(const AccountIdType& AccountId);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(Account* const record);
		void Erase(Account* const record);
		bool CheckInsert(Account* const record);
		// 调用方必须已持有 SharedMutex 独占锁；返回表内活记录，不存在则为空
		Account* Find(const Account* record);
		// 调用方必须已持有 SharedMutex 独占锁：新行换掉旧行，旧行退休
		void Replace(Account* const liveRecord, const Account& newRecord);
		// 调用方必须已持有 SharedMutex 独占锁：整表清空，旧槽数组连同活记录一起退休
		void Clear();

		template <typename Visitor>
		void ForEachRecord(Visitor&& visitor) const
		{
			const SlotArray* slotArray = root_.load(std::memory_order_acquire);
			for (std::size_t position = 0; position < slotArray->Capacity; ++position)
			{
				Account* record = slotArray->Slots[position].Record.load(std::memory_order_acquire);
				if (record != nullptr && record != TombstoneSlot() && !visitor(record))
				{
					return;
				}
			}
		}

	private:
		friend class AccountTable;

		struct SlotArray
		{
			AccountSlot* Slots;
			std::size_t Capacity;
		};

		static constexpr std::size_t InitialSlotCapacity = 1024;

		static std::size_t HashKey(const Account* record) noexcept;
		static Account* TombstoneSlot() noexcept;
		static bool IsLiveSlot(const Account* record) noexcept;
		static SlotArray* CreateSlotArray(std::size_t capacity);
		static std::size_t FindFreeSlotPositionIn(const SlotArray& slotArray, const Account* keyRecord);
		static void ReclaimSlotArray(void* object) noexcept;
		static void ReclaimSlotArrayWithRecords(void* object) noexcept;
		static void ReclaimRecord(void* object) noexcept;
		const Account* FindLiveRecord(const Account* keyRecord) const;
		std::size_t FindOccupiedSlotPosition(const Account* record) const;
		std::size_t FindFreeSlotPosition(const Account* keyRecord) const;
		void RebuildForInsertIfNeeded();
		void Rebuild(std::size_t capacity);

		AccountTable* table_;
		std::atomic<SlotArray*> root_;
		std::size_t rowCount_ = 0;
		std::size_t usedSlotCount_ = 0;
		Rcu::ReaderEpoch epoch_;
	};

	struct CapitalSlot
	{
		std::atomic<Capital*> Record{ nullptr };
	};

	class CapitalTable;

	// 主键索引：开放寻址的原子槽位数组，槽数组由根指针原子发布。
	// Select 无锁读拷贝；SelectAll 沿用 Mdb 的表级读租约，期间写者被挡在 SharedMutex 外。
	class CapitalTablePrimaryKey
	{
	public:
		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = Capital*;
			using difference_type = std::ptrdiff_t;
			using pointer = Capital* const*;
			using reference = Capital*;

			iterator() noexcept = default;
			Capital* operator*() const noexcept;
			iterator& operator++() noexcept;
			iterator operator++(int) noexcept;
			bool operator==(const iterator& other) const noexcept;
			bool operator!=(const iterator& other) const noexcept;

		private:
			friend class CapitalTablePrimaryKey;

			iterator(const CapitalSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept;
			void SkipUnoccupiedSlots() noexcept;

			const CapitalSlot* slotBegin_ = nullptr;
			std::size_t slotCount_ = 0;
			std::size_t position_ = 0;
		};

		explicit CapitalTablePrimaryKey(CapitalTable* tableOwner);
		~CapitalTablePrimaryKey();
		CapitalTablePrimaryKey(const CapitalTablePrimaryKey&) = delete;
		CapitalTablePrimaryKey& operator=(const CapitalTablePrimaryKey&) = delete;

		std::optional<Capital> Select(const DateType& TradingDay, const AccountIdType& AccountId);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(Capital* const record);
		void Erase(Capital* const record);
		bool CheckInsert(Capital* const record);
		// 调用方必须已持有 SharedMutex 独占锁；返回表内活记录，不存在则为空
		Capital* Find(const Capital* record);
		// 调用方必须已持有 SharedMutex 独占锁：新行换掉旧行，旧行退休
		void Replace(Capital* const liveRecord, const Capital& newRecord);
		// 调用方必须已持有 SharedMutex 独占锁：整表清空，旧槽数组连同活记录一起退休
		void Clear();

		template <typename Visitor>
		void ForEachRecord(Visitor&& visitor) const
		{
			const SlotArray* slotArray = root_.load(std::memory_order_acquire);
			for (std::size_t position = 0; position < slotArray->Capacity; ++position)
			{
				Capital* record = slotArray->Slots[position].Record.load(std::memory_order_acquire);
				if (record != nullptr && record != TombstoneSlot() && !visitor(record))
				{
					return;
				}
			}
		}

	private:
		friend class CapitalTable;

		struct SlotArray
		{
			CapitalSlot* Slots;
			std::size_t Capacity;
		};

		static constexpr std::size_t InitialSlotCapacity = 1024;

		static std::size_t HashKey(const Capital* record) noexcept;
		static Capital* TombstoneSlot() noexcept;
		static bool IsLiveSlot(const Capital* record) noexcept;
		static SlotArray* CreateSlotArray(std::size_t capacity);
		static std::size_t FindFreeSlotPositionIn(const SlotArray& slotArray, const Capital* keyRecord);
		static void ReclaimSlotArray(void* object) noexcept;
		static void ReclaimSlotArrayWithRecords(void* object) noexcept;
		static void ReclaimRecord(void* object) noexcept;
		const Capital* FindLiveRecord(const Capital* keyRecord) const;
		std::size_t FindOccupiedSlotPosition(const Capital* record) const;
		std::size_t FindFreeSlotPosition(const Capital* keyRecord) const;
		void RebuildForInsertIfNeeded();
		void Rebuild(std::size_t capacity);

		CapitalTable* table_;
		std::atomic<SlotArray*> root_;
		std::size_t rowCount_ = 0;
		std::size_t usedSlotCount_ = 0;
		Rcu::ReaderEpoch epoch_;
	};

	struct PositionSlot
	{
		std::atomic<Position*> Record{ nullptr };
	};

	class PositionTable;

	// 主键索引：开放寻址的原子槽位数组，槽数组由根指针原子发布。
	// Select 无锁读拷贝；SelectAll 沿用 Mdb 的表级读租约，期间写者被挡在 SharedMutex 外。
	class PositionTablePrimaryKey
	{
	public:
		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = Position*;
			using difference_type = std::ptrdiff_t;
			using pointer = Position* const*;
			using reference = Position*;

			iterator() noexcept = default;
			Position* operator*() const noexcept;
			iterator& operator++() noexcept;
			iterator operator++(int) noexcept;
			bool operator==(const iterator& other) const noexcept;
			bool operator!=(const iterator& other) const noexcept;

		private:
			friend class PositionTablePrimaryKey;

			iterator(const PositionSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept;
			void SkipUnoccupiedSlots() noexcept;

			const PositionSlot* slotBegin_ = nullptr;
			std::size_t slotCount_ = 0;
			std::size_t position_ = 0;
		};

		explicit PositionTablePrimaryKey(PositionTable* tableOwner);
		~PositionTablePrimaryKey();
		PositionTablePrimaryKey(const PositionTablePrimaryKey&) = delete;
		PositionTablePrimaryKey& operator=(const PositionTablePrimaryKey&) = delete;

		std::optional<Position> Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(Position* const record);
		void Erase(Position* const record);
		bool CheckInsert(Position* const record);
		// 调用方必须已持有 SharedMutex 独占锁；返回表内活记录，不存在则为空
		Position* Find(const Position* record);
		// 调用方必须已持有 SharedMutex 独占锁：新行换掉旧行，旧行退休
		void Replace(Position* const liveRecord, const Position& newRecord);
		// 调用方必须已持有 SharedMutex 独占锁：整表清空，旧槽数组连同活记录一起退休
		void Clear();

		template <typename Visitor>
		void ForEachRecord(Visitor&& visitor) const
		{
			const SlotArray* slotArray = root_.load(std::memory_order_acquire);
			for (std::size_t position = 0; position < slotArray->Capacity; ++position)
			{
				Position* record = slotArray->Slots[position].Record.load(std::memory_order_acquire);
				if (record != nullptr && record != TombstoneSlot() && !visitor(record))
				{
					return;
				}
			}
		}

	private:
		friend class PositionTable;

		struct SlotArray
		{
			PositionSlot* Slots;
			std::size_t Capacity;
		};

		static constexpr std::size_t InitialSlotCapacity = 1024;

		static std::size_t HashKey(const Position* record) noexcept;
		static Position* TombstoneSlot() noexcept;
		static bool IsLiveSlot(const Position* record) noexcept;
		static SlotArray* CreateSlotArray(std::size_t capacity);
		static std::size_t FindFreeSlotPositionIn(const SlotArray& slotArray, const Position* keyRecord);
		static void ReclaimSlotArray(void* object) noexcept;
		static void ReclaimSlotArrayWithRecords(void* object) noexcept;
		static void ReclaimRecord(void* object) noexcept;
		const Position* FindLiveRecord(const Position* keyRecord) const;
		std::size_t FindOccupiedSlotPosition(const Position* record) const;
		std::size_t FindFreeSlotPosition(const Position* keyRecord) const;
		void RebuildForInsertIfNeeded();
		void Rebuild(std::size_t capacity);

		PositionTable* table_;
		std::atomic<SlotArray*> root_;
		std::size_t rowCount_ = 0;
		std::size_t usedSlotCount_ = 0;
		Rcu::ReaderEpoch epoch_;
	};

	struct PositionDetailSlot
	{
		std::atomic<PositionDetail*> Record{ nullptr };
	};

	class PositionDetailTable;

	// 主键索引：开放寻址的原子槽位数组，槽数组由根指针原子发布。
	// Select 无锁读拷贝；SelectAll 沿用 Mdb 的表级读租约，期间写者被挡在 SharedMutex 外。
	class PositionDetailTablePrimaryKey
	{
	public:
		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = PositionDetail*;
			using difference_type = std::ptrdiff_t;
			using pointer = PositionDetail* const*;
			using reference = PositionDetail*;

			iterator() noexcept = default;
			PositionDetail* operator*() const noexcept;
			iterator& operator++() noexcept;
			iterator operator++(int) noexcept;
			bool operator==(const iterator& other) const noexcept;
			bool operator!=(const iterator& other) const noexcept;

		private:
			friend class PositionDetailTablePrimaryKey;

			iterator(const PositionDetailSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept;
			void SkipUnoccupiedSlots() noexcept;

			const PositionDetailSlot* slotBegin_ = nullptr;
			std::size_t slotCount_ = 0;
			std::size_t position_ = 0;
		};

		explicit PositionDetailTablePrimaryKey(PositionDetailTable* tableOwner);
		~PositionDetailTablePrimaryKey();
		PositionDetailTablePrimaryKey(const PositionDetailTablePrimaryKey&) = delete;
		PositionDetailTablePrimaryKey& operator=(const PositionDetailTablePrimaryKey&) = delete;

		std::optional<PositionDetail> Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection, const DateType& OpenDate, const TradeIdType& TradeId);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(PositionDetail* const record);
		void Erase(PositionDetail* const record);
		bool CheckInsert(PositionDetail* const record);
		// 调用方必须已持有 SharedMutex 独占锁；返回表内活记录，不存在则为空
		PositionDetail* Find(const PositionDetail* record);
		// 调用方必须已持有 SharedMutex 独占锁：新行换掉旧行，旧行退休
		void Replace(PositionDetail* const liveRecord, const PositionDetail& newRecord);
		// 调用方必须已持有 SharedMutex 独占锁：整表清空，旧槽数组连同活记录一起退休
		void Clear();

		template <typename Visitor>
		void ForEachRecord(Visitor&& visitor) const
		{
			const SlotArray* slotArray = root_.load(std::memory_order_acquire);
			for (std::size_t position = 0; position < slotArray->Capacity; ++position)
			{
				PositionDetail* record = slotArray->Slots[position].Record.load(std::memory_order_acquire);
				if (record != nullptr && record != TombstoneSlot() && !visitor(record))
				{
					return;
				}
			}
		}

	private:
		friend class PositionDetailTable;

		struct SlotArray
		{
			PositionDetailSlot* Slots;
			std::size_t Capacity;
		};

		static constexpr std::size_t InitialSlotCapacity = 1024;

		static std::size_t HashKey(const PositionDetail* record) noexcept;
		static PositionDetail* TombstoneSlot() noexcept;
		static bool IsLiveSlot(const PositionDetail* record) noexcept;
		static SlotArray* CreateSlotArray(std::size_t capacity);
		static std::size_t FindFreeSlotPositionIn(const SlotArray& slotArray, const PositionDetail* keyRecord);
		static void ReclaimSlotArray(void* object) noexcept;
		static void ReclaimSlotArrayWithRecords(void* object) noexcept;
		static void ReclaimRecord(void* object) noexcept;
		const PositionDetail* FindLiveRecord(const PositionDetail* keyRecord) const;
		std::size_t FindOccupiedSlotPosition(const PositionDetail* record) const;
		std::size_t FindFreeSlotPosition(const PositionDetail* keyRecord) const;
		void RebuildForInsertIfNeeded();
		void Rebuild(std::size_t capacity);

		PositionDetailTable* table_;
		std::atomic<SlotArray*> root_;
		std::size_t rowCount_ = 0;
		std::size_t usedSlotCount_ = 0;
		Rcu::ReaderEpoch epoch_;
	};

	struct OrderSlot
	{
		std::atomic<Order*> Record{ nullptr };
	};

	class OrderTable;

	// 主键索引：开放寻址的原子槽位数组，槽数组由根指针原子发布。
	// Select 无锁读拷贝；SelectAll 沿用 Mdb 的表级读租约，期间写者被挡在 SharedMutex 外。
	class OrderTablePrimaryKey
	{
	public:
		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = Order*;
			using difference_type = std::ptrdiff_t;
			using pointer = Order* const*;
			using reference = Order*;

			iterator() noexcept = default;
			Order* operator*() const noexcept;
			iterator& operator++() noexcept;
			iterator operator++(int) noexcept;
			bool operator==(const iterator& other) const noexcept;
			bool operator!=(const iterator& other) const noexcept;

		private:
			friend class OrderTablePrimaryKey;

			iterator(const OrderSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept;
			void SkipUnoccupiedSlots() noexcept;

			const OrderSlot* slotBegin_ = nullptr;
			std::size_t slotCount_ = 0;
			std::size_t position_ = 0;
		};

		explicit OrderTablePrimaryKey(OrderTable* tableOwner);
		~OrderTablePrimaryKey();
		OrderTablePrimaryKey(const OrderTablePrimaryKey&) = delete;
		OrderTablePrimaryKey& operator=(const OrderTablePrimaryKey&) = delete;

		std::optional<Order> Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const OrderIdType& OrderId);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(Order* const record);
		void Erase(Order* const record);
		bool CheckInsert(Order* const record);
		// 调用方必须已持有 SharedMutex 独占锁；返回表内活记录，不存在则为空
		Order* Find(const Order* record);
		// 调用方必须已持有 SharedMutex 独占锁：新行换掉旧行，旧行退休
		void Replace(Order* const liveRecord, const Order& newRecord);
		// 调用方必须已持有 SharedMutex 独占锁：整表清空，旧槽数组连同活记录一起退休
		void Clear();

		template <typename Visitor>
		void ForEachRecord(Visitor&& visitor) const
		{
			const SlotArray* slotArray = root_.load(std::memory_order_acquire);
			for (std::size_t position = 0; position < slotArray->Capacity; ++position)
			{
				Order* record = slotArray->Slots[position].Record.load(std::memory_order_acquire);
				if (record != nullptr && record != TombstoneSlot() && !visitor(record))
				{
					return;
				}
			}
		}

	private:
		friend class OrderTable;

		struct SlotArray
		{
			OrderSlot* Slots;
			std::size_t Capacity;
		};

		static constexpr std::size_t InitialSlotCapacity = 1024;

		static std::size_t HashKey(const Order* record) noexcept;
		static Order* TombstoneSlot() noexcept;
		static bool IsLiveSlot(const Order* record) noexcept;
		static SlotArray* CreateSlotArray(std::size_t capacity);
		static std::size_t FindFreeSlotPositionIn(const SlotArray& slotArray, const Order* keyRecord);
		static void ReclaimSlotArray(void* object) noexcept;
		static void ReclaimSlotArrayWithRecords(void* object) noexcept;
		static void ReclaimRecord(void* object) noexcept;
		const Order* FindLiveRecord(const Order* keyRecord) const;
		std::size_t FindOccupiedSlotPosition(const Order* record) const;
		std::size_t FindFreeSlotPosition(const Order* keyRecord) const;
		void RebuildForInsertIfNeeded();
		void Rebuild(std::size_t capacity);

		OrderTable* table_;
		std::atomic<SlotArray*> root_;
		std::size_t rowCount_ = 0;
		std::size_t usedSlotCount_ = 0;
		Rcu::ReaderEpoch epoch_;
	};

	struct TradeSlot
	{
		std::atomic<Trade*> Record{ nullptr };
	};

	class TradeTable;

	// 主键索引：开放寻址的原子槽位数组，槽数组由根指针原子发布。
	// Select 无锁读拷贝；SelectAll 沿用 Mdb 的表级读租约，期间写者被挡在 SharedMutex 外。
	class TradeTablePrimaryKey
	{
	public:
		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = Trade*;
			using difference_type = std::ptrdiff_t;
			using pointer = Trade* const*;
			using reference = Trade*;

			iterator() noexcept = default;
			Trade* operator*() const noexcept;
			iterator& operator++() noexcept;
			iterator operator++(int) noexcept;
			bool operator==(const iterator& other) const noexcept;
			bool operator!=(const iterator& other) const noexcept;

		private:
			friend class TradeTablePrimaryKey;

			iterator(const TradeSlot* slotBegin, std::size_t slotCount, std::size_t position) noexcept;
			void SkipUnoccupiedSlots() noexcept;

			const TradeSlot* slotBegin_ = nullptr;
			std::size_t slotCount_ = 0;
			std::size_t position_ = 0;
		};

		explicit TradeTablePrimaryKey(TradeTable* tableOwner);
		~TradeTablePrimaryKey();
		TradeTablePrimaryKey(const TradeTablePrimaryKey&) = delete;
		TradeTablePrimaryKey& operator=(const TradeTablePrimaryKey&) = delete;

		std::optional<Trade> Select(const DateType& TradingDay, const ExchangeIdType& ExchangeId, const TradeIdType& TradeId, const DirectionType& Direction);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(Trade* const record);
		void Erase(Trade* const record);
		bool CheckInsert(Trade* const record);
		// 调用方必须已持有 SharedMutex 独占锁；返回表内活记录，不存在则为空
		Trade* Find(const Trade* record);
		// 调用方必须已持有 SharedMutex 独占锁：新行换掉旧行，旧行退休
		void Replace(Trade* const liveRecord, const Trade& newRecord);
		// 调用方必须已持有 SharedMutex 独占锁：整表清空，旧槽数组连同活记录一起退休
		void Clear();

		template <typename Visitor>
		void ForEachRecord(Visitor&& visitor) const
		{
			const SlotArray* slotArray = root_.load(std::memory_order_acquire);
			for (std::size_t position = 0; position < slotArray->Capacity; ++position)
			{
				Trade* record = slotArray->Slots[position].Record.load(std::memory_order_acquire);
				if (record != nullptr && record != TombstoneSlot() && !visitor(record))
				{
					return;
				}
			}
		}

	private:
		friend class TradeTable;

		struct SlotArray
		{
			TradeSlot* Slots;
			std::size_t Capacity;
		};

		static constexpr std::size_t InitialSlotCapacity = 1024;

		static std::size_t HashKey(const Trade* record) noexcept;
		static Trade* TombstoneSlot() noexcept;
		static bool IsLiveSlot(const Trade* record) noexcept;
		static SlotArray* CreateSlotArray(std::size_t capacity);
		static std::size_t FindFreeSlotPositionIn(const SlotArray& slotArray, const Trade* keyRecord);
		static void ReclaimSlotArray(void* object) noexcept;
		static void ReclaimSlotArrayWithRecords(void* object) noexcept;
		static void ReclaimRecord(void* object) noexcept;
		const Trade* FindLiveRecord(const Trade* keyRecord) const;
		std::size_t FindOccupiedSlotPosition(const Trade* record) const;
		std::size_t FindFreeSlotPosition(const Trade* keyRecord) const;
		void RebuildForInsertIfNeeded();
		void Rebuild(std::size_t capacity);

		TradeTable* table_;
		std::atomic<SlotArray*> root_;
		std::size_t rowCount_ = 0;
		std::size_t usedSlotCount_ = 0;
		Rcu::ReaderEpoch epoch_;
	};

}
