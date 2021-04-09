#include "Application.h"

#include "ThreadRouter.h"

Application::Application() : m_thread_pool(std::make_unique<ThreadPool>())
{
}

void Application::AddReceiveCallBack(const int msgID, void* pBase, std::function<void(void)> call_func)
{

}

void Application::SendMsgToActor(const std::string& sender_uuid, const std::string& receiver_uuid, std::function<void(void)> call_func, std::function<void(void)> callback)
{

}

void Application::RpcCall()
{

}

void Application::ResponseMsg()
{

}

void Application::PushMsg()
{

}

void Application::BroadcastMsg()
{

}

void Application::AddActorToThreadCell(std::shared_ptr<IActor> ptr_actor)
{
	m_thread_pool->AddActorToThreadCell(ptr_actor);
}

void Application::RemoveActorToThreadCell(const std::string& uuid)
{

}

void Application::StartThreadPool()
{
	m_thread_pool->StartThreadPool();
}