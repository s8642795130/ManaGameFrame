#pragma once
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue
{
private:
	std::mutex m_mut;
	std::queue<T> m_data_queue;
	std::condition_variable m_data_cond;
public:
	ThreadSafeQueue() = default;

	ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

	void Push(T value)
	{
		std::lock_guard<std::mutex> lk(m_mut);
		m_data_queue.push(value);
		m_data_cond.notify_one();
	}

	void WaitAndPop(T& value)
	{
		std::unique_lock<std::mutex> lk(m_mut);
		m_data_cond.wait(lk, [this] { return !m_data_queue.empty(); });
		value = m_data_queue.front();
		m_data_queue.pop();
	}

	std::shared_ptr<T> WaitAndPop()
	{
		std::unique_lock<std::mutex> lk(m_mut);
		m_data_cond.wait(lk, [this] { return !m_data_queue.empty(); });
		std::shared_ptr<T> res = std::make_shared<T>(m_data_queue.front());
		m_data_queue.pop();
		return res;
	}

	bool TryPop(T& value)
	{
		std::lock_guard<std::mutex> lk(m_mut);
		if (m_data_queue.empty())
		{
			return false;
		}
		value = m_data_queue.front();
		m_data_queue.pop();
		return true;
	}

	bool Empty() const
	{
		std::lock_guard<std::mutex> lk(m_mut);
		return m_data_queue.empty();
	}
};

