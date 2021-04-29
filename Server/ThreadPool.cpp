#include "ThreadPool.h"
#include "ThreadRouter.h"
#include "ClientNetControlActor.h"

void ThreadPool::StartThreadPool()
{
	m_thread_count = std::thread::hardware_concurrency();
	std::cout << "Thread count: " << m_thread_count << " run" << std::endl;
	for (unsigned int i = 0; i < m_thread_count; ++i)
	{
		m_thread_pool.push_back(std::make_shared<ThreadCell>());
	}

	// set threadRouter thread count
	ThreadRouter::SetThreadCountNum(m_thread_count);

	//
	AddClientProcActorToThread();
}

void ThreadPool::AddClientProcActorToThread()
{
	// add clientNet message proc actor to thread array
	for (unsigned int i = 0; i < m_thread_count; ++i)
	{
		std::shared_ptr<ClientNetControlActor> ptr_actor{ std::make_shared<ClientNetControlActor>() };
		m_arr_thread_proc_actor_uuid.push_back(ptr_actor->GetUUID());
		m_thread_pool[i]->AddActorToMap(ptr_actor);
	}
}

const std::string& ThreadPool::GetThreadActorUUID(int index)
{
	return m_arr_thread_proc_actor_uuid[index];
}

void ThreadPool::AddActorToThreadCell(std::shared_ptr<IActor> ptr_actor)
{
	unsigned long thread_index{ ThreadRouter::GetThreadIndexByUUID(ptr_actor->GetUUID()) };
	m_thread_pool[thread_index]->AddActorToMap(ptr_actor);
}

void ThreadPool::AddActorMsgToThreadCell(std::unique_ptr<IActorMsg>& ptr_actor_msg)
{
	unsigned long thread_index{ ThreadRouter::GetThreadIndexByUUID(ptr_actor_msg->GetReceiverActorUUID()) };
	m_thread_pool[thread_index]->AddActorMsgToMap(ptr_actor_msg);
}