#include "ThreadPool.h"
#include "ThreadRouter.h"

void ThreadPool::StartThreadPool()
{
	m_thread_count = std::thread::hardware_concurrency();
	std::cout << "Thread count: " << m_thread_count << " ¸öÏß³Ì" << std::endl;
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