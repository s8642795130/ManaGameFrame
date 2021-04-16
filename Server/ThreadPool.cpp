#include "ThreadPool.h"
#include "ThreadRouter.h"

void ThreadPool::StartThreadPool()
{
	m_thread_count = std::thread::hardware_concurrency();
	std::cout << "Thread count: " << m_thread_count << " run" << std::endl;
	for (unsigned int i = 0; i < m_thread_count; ++i)
	{
		m_thread_pool.push_back(std::make_shared<ThreadCell>());
	}
}

void ThreadPool::AddActorToThreadCell(std::shared_ptr<IActor> ptr_actor)
{
	unsigned long thread_index{ ThreadRouter::GetThreadIndexByUUID(m_thread_count, ptr_actor->GetUUID()) };
	m_thread_pool[thread_index]->AddActorToMap(ptr_actor);
}

void ThreadPool::AddActorMsgToThreadCell(std::unique_ptr<IActorMsg>& ptr_actor_msg)
{
	unsigned long thread_index{ ThreadRouter::GetThreadIndexByUUID(m_thread_count, ptr_actor_msg->GetReceiverActorUUID()) };
	m_thread_pool[thread_index]->AddActorMsgToMap(ptr_actor_msg);
}