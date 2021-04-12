#include "Application.h"

#include "ThreadRouter.h"

Application::Application() : m_thread_pool(std::make_unique<ThreadPool>()), m_server_net(std::make_unique<ServerNet<Client>>())
{
}

void Application::AddReceiveCallBack(const int msg_id, std::function<void(ClientDescriptor*, int, char*)> call_func)
{
	return m_server_net->AddReceiveCallBack(msg_id, call_func);
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

void Application::StartNetwork()
{
	m_server_net->StartNetwork("0.0.0.0", 3010, 30);
}