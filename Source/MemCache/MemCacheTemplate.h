#pragma once
#include <list>

template <typename T>
class MemCacheTemplate
{
public:
	MemCacheTemplate()
	{
	}
	~MemCacheTemplate()
	{
		for (auto& item : m_Items)
		{
			delete item;
		}

		m_Items.clear();
	}
	MemCacheTemplate(MemCacheTemplate&) = delete;
	MemCacheTemplate& operator=(MemCacheTemplate&) = delete;

	void Init(int initNum = 64)
	{
		for (auto i = 0; i < initNum; i++)
		{
			m_Items.push_back(new T());
		}
	}

	T* Allocate()
	{
		if (!m_Items.empty())
		{
			auto item = m_Items.front();
			m_Items.pop_front();
			return item;
		}
		return new T();
	}
	void Free(T* item)
	{
		m_Items.push_back(item);
	}

private:
	std::list<T*> m_Items;
};
