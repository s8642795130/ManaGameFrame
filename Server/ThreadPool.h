#pragma once
#include <thread>
#include <memory>
#include <iostream>
#include <map>
#include <functional>

#include "ThreadSafeQueue.h"
#include "ThreadSafeMap.h"
#include "IActor.h"
#include "IActorMsg.h"

/// <summary>
/// ThreadTask
/// </summary>
class ThreadTask
{
private:
	std::unique_ptr<IActorMsg> m_actor_msg;
public:
	ThreadTask(std::unique_ptr<IActorMsg>& actor_msg) : 
		m_actor_msg(std::move(actor_msg))
	{

	}

	const std::string& GetReceiverActorUUID()
	{
		return m_actor_msg->GetReceiverActorUUID();
	}

	void Call(std::shared_ptr<IActor> ptr)
	{
		m_actor_msg->Act(ptr.get());
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
	ThreadSafeQueue<std::shared_ptr<ThreadTask>> m_task_queue;

	void WorkThread()
	{
		while (true)
		{
			std::cout << "run thread" << std::endl;
			// m_task_queue.WaitAndPop(task);
			std::shared_ptr<ThreadTask> ptr_task = *m_task_queue.WaitAndPop();
			//
			std::shared_ptr<IActor> ptr_actor = m_map_actor.GetElement(ptr_task->GetReceiverActorUUID());
			ptr_task->Call(ptr_actor);
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

	void AddActorMsgToMap(std::unique_ptr<IActorMsg>& ptr_actor_msg)
	{
		std::shared_ptr<ThreadTask> ptr_task = std::make_shared<ThreadTask>(ptr_actor_msg);
		m_task_queue.Push(ptr_task);
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
	void AddActorMsgToThreadCell(std::unique_ptr<IActorMsg>& ptr_actor_msg);
};

