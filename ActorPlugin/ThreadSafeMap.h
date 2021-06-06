#pragma once
#include <map>
#include <mutex>

template<typename T, typename Element>
class ThreadSafeMap
{
private:
	std::mutex m_mut;
	std::map<T, Element> m_data_map;
public:
	ThreadSafeMap() = default;
	ThreadSafeMap& operator=(const ThreadSafeMap&) = delete;

	void Emplace(T value, Element element)
	{
		std::lock_guard<std::mutex> lk(m_mut);
		// m_data_map.count(value);
		m_data_map.emplace(value, element);
	}

	Element& GetElement(const T value)
	{
		std::lock_guard<std::mutex> lk(m_mut);
		return m_data_map[value];
	}

	void Erase(const T& value)
	{
		std::lock_guard<std::mutex> lk(m_mut);
		m_data_map.erase(value);
	}
};
