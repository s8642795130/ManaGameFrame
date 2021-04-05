#pragma once
#include <thread>
#include <memory>
#include <iostream>
#include <map>
#include <functional>

// #include "ThreadSafeDeque.h"
#include "ThreadSafeQueue.h"
#include "ThreadSafeMap.h"
#include "IActor.h"

/// <summary>
/// ThreadTask
/// </summary>
class ThreadTask
{
private:
	std::function<void()> m_call;
	std::function<void()> m_callback;
public:
	void Call()
	{
		m_call();
		m_callback();
	}
};

/// <summary>
/// ThreadCell
/// </summary>
class ThreadCell
{
private:
	std::unique_ptr<std::thread> m_thread;
	ThreadSafeMap<std::string, std::shared_ptr<IActor>> m_map_actor;
	// ThreadSafeDeque<ThreadTask> m_task_deque;
	ThreadSafeQueue<ThreadTask> m_task_queue;

	void WorkThread()
	{
		while (true)
		{
			std::cout << "run thread" << std::endl;
			ThreadTask task;
			m_task_queue.WaitAndPop(task);
			task.Call();
		}
	}
public:
	ThreadCell()
	{
		m_thread = std::make_unique<std::thread>(&ThreadCell::WorkThread, this);
	}

	void AddActorToMap(std::shared_ptr<IActor> ptr_actor)
	{
		m_map_actor.Emplace(ptr_actor->GetUUID(), ptr_actor);
	}

	void DelActorFromMap(std::string& str_key)
	{
		m_map_actor.Erase(str_key);
	}

	virtual ~ThreadCell()
	{
		if (m_thread->joinable())
		{
			m_thread->join();
		}
	}
};

/// <summary>
/// ThreadPool
/// </summary>
class ThreadPool
{
private:
	unsigned int m_thread_count = 0;
	std::vector<std::shared_ptr<ThreadCell>> m_thread_pool;
public:
	void StartThreadPool();
	void AddActorToThreadCell(std::shared_ptr<IActor> ptr_actor);
};

