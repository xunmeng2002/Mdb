// 本文件由 ../Templates/Cpp/Mdb/Rcu/RcuEpoch.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "Rcu/RcuEpoch.h"
#include <cstdio>
#include <cstdlib>
#include <utility>


namespace Mdb::Rcu
{
	ReaderEpoch::Guard::Guard(ReaderEpoch& owner, std::size_t readerIndex) noexcept
		: slot_(&owner.readerSlots_[readerIndex])
	{
		// 双检：登记后若代数已推进，写者的扫描可能发生在登记之前，必须重来
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

	ReaderEpoch::Guard::Guard(Guard&& other) noexcept
		: slot_(other.slot_)
	{
		other.slot_ = nullptr;
	}

	ReaderEpoch::Guard::~Guard()
	{
		Release();
	}

	void ReaderEpoch::Guard::Release() noexcept
	{
		if (slot_ != nullptr)
		{
			slot_->store(0, std::memory_order_release);
			slot_ = nullptr;
		}
	}

	ReaderEpoch::ReaderEpoch()
		: readerSlots_(MaxReaders)
	{
	}

	ReaderEpoch::~ReaderEpoch()
	{
		ReclaimAllWithoutReaders();
	}

	void ReaderEpoch::Retire(void* object, ReclaimObject reclaimObject)
	{
		retiredObjects_.push_back(RetiredObject{ object, reclaimObject, currentEpoch_.load(std::memory_order_seq_cst) });
	}

	void ReaderEpoch::AdvanceAndReclaim()
	{
		currentEpoch_.fetch_add(1, std::memory_order_seq_cst);
		ReclaimThrough(MinimumRegisteredEpoch());
	}

	void ReaderEpoch::ReclaimAllWithoutReaders() noexcept
	{
		for (const RetiredObject& retired : retiredObjects_)
		{
			retired.Reclaim(retired.Object);
		}
		retiredObjects_.clear();
	}

	std::size_t ReaderEpoch::PendingRetireCount() const noexcept
	{
		return retiredObjects_.size();
	}

	std::uint64_t ReaderEpoch::MinimumRegisteredEpoch() const noexcept
	{
		std::uint64_t minimumEpoch = currentEpoch_.load(std::memory_order_seq_cst);
		for (const std::atomic<std::uint64_t>& readerSlot : readerSlots_)
		{
			const std::uint64_t registeredEpoch = readerSlot.load(std::memory_order_seq_cst);
			if (registeredEpoch != 0 && registeredEpoch < minimumEpoch)
			{
				minimumEpoch = registeredEpoch;
			}
		}
		return minimumEpoch;
	}

	void ReaderEpoch::ReclaimThrough(std::uint64_t reclaimableEpoch) noexcept
	{
		for (auto retired = retiredObjects_.begin(); retired != retiredObjects_.end();)
		{
			if (retired->RetiredAtEpoch < reclaimableEpoch)
			{
				retired->Reclaim(retired->Object);
				retired = retiredObjects_.erase(retired);
			}
			else
			{
				++retired;
			}
		}
	}

	std::size_t CurrentReaderIndex()
	{
		thread_local const std::size_t readerIndex = []() -> std::size_t
		{
			static std::atomic<std::size_t> nextReaderIndex{ 0 };
			const std::size_t index = nextReaderIndex.fetch_add(1, std::memory_order_relaxed);
			if (index >= ReaderEpoch::MaxReaders)
			{
				std::fprintf(stderr, "Mdb::Rcu: reader threads exceed MaxReaders(%zu)\n", ReaderEpoch::MaxReaders);
				std::abort();
			}
			return index;
		}();
		return readerIndex;
	}
}
