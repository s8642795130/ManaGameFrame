#include "ThreadPoolModule.h"
#include "ThreadRouter.h"

void ThreadPoolModule::Init()
{
	StartThreadPool();
}

void ThreadPoolModule::AfterInit()
{

}

// function

unsigned int ThreadRouter::m_thread_count;

void ThreadPoolModule::StartThreadPool()
{
	// get cpu thread
	m_thread_count = std::thread::hardware_concurrency();
	std::cout << "Thread count: " << m_thread_count << std::endl;

	// create thread cell
	for (unsigned int i = 0; i < m_thread_count; ++i)
	{
		m_thread_pool.push_back(std::make_shared<ThreadCell>());
	}

	// set threadRouter thread count
	ThreadRouter::SetThreadCountNum(m_thread_count);
}

void ThreadPoolModule::AddActorToThreadCell(std::shared_ptr<IActor> ptr_actor)
{
	auto thread_index{ ThreadRouter::GetThreadIndexByUUID(ptr_actor->GetUUID()) };
	m_thread_pool[thread_index]->AddActorToMap(ptr_actor);
}

void ThreadPoolModule::AddActorMsgToThreadCell(std::unique_ptr<IActorMsg>& ptr_actor_msg)
{
	auto thread_index{ ThreadRouter::GetThreadIndexByUUID(ptr_actor_msg->GetReceiverActorUUID()) };
	m_thread_pool[thread_index]->AddActorMsgToMap(ptr_actor_msg);
}

void ThreadPoolModule::RemoveActorFromThreadCell(const std::string& uuid)
{
	auto thread_index{ ThreadRouter::GetThreadIndexByUUID(uuid) };
	m_thread_pool[thread_index]->RemoveActorFromMap(uuid);
}