// 本文件由 ../Templates/Cpp/Mdb/Rcu/RcuEpoch.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <vector>


namespace Mdb::Rcu
{
	// 读者登记：每个线程固定占一个槽位，进入临界区时写入当前代数。
	// 写者按「最小登记代数」判断退休对象是否还有人可能引用。
	// 契约：Retire / AdvanceAndReclaim / ReclaimAllWithoutReaders 只由单写者线程调用。
	class ReaderEpoch
	{
	public:
		static constexpr std::size_t MaxReaders = 64;
		using ReclaimObject = void (*)(void* object);

		class Guard
		{
		public:
			Guard() noexcept = default;
			Guard(ReaderEpoch& owner, std::size_t readerIndex) noexcept;
			~Guard();
			Guard(const Guard&) = delete;
			Guard& operator=(const Guard&) = delete;
			Guard(Guard&& other) noexcept;
			Guard& operator=(Guard&&) = delete;

		private:
			void Release() noexcept;

			std::atomic<std::uint64_t>* slot_ = nullptr;
		};

		ReaderEpoch();
		~ReaderEpoch();
		ReaderEpoch(const ReaderEpoch&) = delete;
		ReaderEpoch& operator=(const ReaderEpoch&) = delete;

		void Retire(void* object, ReclaimObject reclaimObject);
		void AdvanceAndReclaim();
		// 只能在表已不可达（析构）时调用：此后不再有读者，退休对象直接释放
		void ReclaimAllWithoutReaders() noexcept;
		std::size_t PendingRetireCount() const noexcept;

	private:
		struct RetiredObject
		{
			void* Object;
			ReclaimObject Reclaim;
			std::uint64_t RetiredAtEpoch;
		};

		std::uint64_t MinimumRegisteredEpoch() const noexcept;
		void ReclaimThrough(std::uint64_t reclaimableEpoch) noexcept;

		std::atomic<std::uint64_t> currentEpoch_{ 1 };
		std::vector<std::atomic<std::uint64_t>> readerSlots_;
		std::vector<RetiredObject> retiredObjects_;
	};

	// 线程固定的登记槽位：同一线程访问任意 ReaderEpoch 都取同一序号。
	// 槽位一次性分配、不回收；线程总数超过 MaxReaders 时直接终止。
	std::size_t CurrentReaderIndex();
}
