#include "ThreadPool.h"
#include "ThreadRouter.h"
#include "ClientNetControlActor.h"

unsigned int ThreadRouter::m_thread_count;

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
}

void ThreadPool::AddActorToThreadCell(std::shared_ptr<IActor> ptr_actor)
{
	auto thread_index{ ThreadRouter::GetThreadIndexByUUID(ptr_actor->GetUUID()) };
	m_thread_pool[thread_index]->AddActorToMap(ptr_actor);
}

void ThreadPool::AddActorMsgToThreadCell(std::unique_ptr<IActorMsg>& ptr_actor_msg)
{
	auto thread_index{ ThreadRouter::GetThreadIndexByUUID(ptr_actor_msg->GetReceiverActorUUID()) };
	m_thread_pool[thread_index]->AddActorMsgToMap(ptr_actor_msg);
}

void ThreadPool::RemoveActorFromThreadCell(const std::string& uuid)
{
	auto thread_index{ ThreadRouter::GetThreadIndexByUUID(uuid) };
	m_thread_pool[thread_index]->RemoveActorFromMap(uuid);
}