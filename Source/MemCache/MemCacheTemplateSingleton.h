#pragma once
#include <list>
#include <mutex>

template <typename T>
class MemCacheTemplateSingleton
{
private:
	MemCacheTemplateSingleton()
	{
	}
	~MemCacheTemplateSingleton()
	{
		for (auto& item : m_Items)
		{
			delete item;
		}

		m_Items.clear();
	}
	MemCacheTemplateSingleton(MemCacheTemplateSingleton&) = delete;
	MemCacheTemplateSingleton& operator=(MemCacheTemplateSingleton&) = delete;
public:
	static MemCacheTemplateSingleton& GetInstance()
	{
		return m_Instance;
	}

	void Init(int initNum = 64)
	{
		for (auto i = 0; i < initNum; i++)
		{
			m_Items.push_back(new T());
		}
	}

	T* Allocate()
	{
		std::lock_guard<std::mutex> guard(m_Mutex);
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
		std::lock_guard<std::mutex> guard(m_Mutex);
		m_Items.push_back(item);
	}

private:
	static MemCacheTemplateSingleton m_Instance;
	std::list<T*> m_Items;
	std::mutex m_Mutex;
};

template<class T>
MemCacheTemplateSingleton<T> MemCacheTemplateSingleton<T>::m_Instance;