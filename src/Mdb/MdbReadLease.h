// 本文件由 ../Templates/Cpp/Mdb/MdbReadLease.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include <shared_mutex>
#include <utility>


namespace Mdb
{
	template <typename Record>
	class TableReadLease
	{
	public:
		TableReadLease() noexcept = default;
		TableReadLease(std::shared_lock<std::shared_mutex>&& lock, const Record* record) noexcept
			: lock_(std::move(lock)), record_(record)
		{
		}
		TableReadLease(TableReadLease&&) noexcept = default;
		TableReadLease& operator=(TableReadLease&&) noexcept = default;
		TableReadLease(const TableReadLease&) = delete;
		TableReadLease& operator=(const TableReadLease&) = delete;

		const Record* Get() const noexcept { return record_; }
		const Record* operator->() const noexcept { return record_; }
		const Record& operator*() const noexcept { return *record_; }
		explicit operator bool() const noexcept { return record_ != nullptr; }

	private:
		std::shared_lock<std::shared_mutex> lock_;
		const Record* record_ = nullptr;
	};

	template <typename Iterator>
	class TableRangeLease
	{
	public:
		TableRangeLease() noexcept = default;
		TableRangeLease(std::shared_lock<std::shared_mutex>&& lock, Iterator first, Iterator last) noexcept
			: lock_(std::move(lock)), first_(first), last_(last)
		{
		}
		TableRangeLease(TableRangeLease&&) noexcept = default;
		TableRangeLease& operator=(TableRangeLease&&) noexcept = default;
		TableRangeLease(const TableRangeLease&) = delete;
		TableRangeLease& operator=(const TableRangeLease&) = delete;

		Iterator begin() const noexcept { return first_; }
		Iterator end() const noexcept { return last_; }
		bool empty() const noexcept { return first_ == last_; }

	private:
		std::shared_lock<std::shared_mutex> lock_;
		Iterator first_{};
		Iterator last_{};
	};
}
