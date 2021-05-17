#include "Application.h"

#include "ThreadRouter.h"

std::shared_ptr<IApplication> IApplication::m_app;

Application::Application() : 
	m_thread_pool(std::make_unique<ThreadPool>()),
	m_server_net(std::make_unique<ServerNet>(m_thread_pool)),
	m_plugin_manager(std::make_unique<PluginManager>())
{
}

void Application::AddReceiveCallBack(const int msg_id, std::function<void(ClientDescriptor*)> call_func)
{
	return m_server_net->AddReceiveCallBack(msg_id, call_func);
}

void Application::SendMsgToActor(std::unique_ptr<IActorMsg>& actor_msg)
{
	m_thread_pool->AddActorMsgToThreadCell(actor_msg);
}

void Application::RPCMsg()
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

void Application::RemoveActorFromThreadCell(const std::string& uuid)
{
	m_thread_pool->RemoveActorFromThreadCell(uuid);
}

std::shared_ptr<IConfigFile> Application::GetConfigPtr()
{
	return m_config_file;
}

//

void Application::LoadConfig(const std::string& server_name)
{
	// load config
	m_config_file->SetServerName(server_name);
	m_config_file->ReadServerConfigFile();
}

void Application::StartLoadAllLibrary(int test_code)
{
	m_plugin_manager->LoadAllPluginLibrary(test_code);
}

void Application::StartConnectServer()
{
	m_server_controller->ConnectServer();
}

//

void Application::LibInit()
{
	m_plugin_manager->Init();
}

void Application::LibExecute()
{
	m_plugin_manager->Execute();
}

//

void Application::StartThreadPool()
{
	m_thread_pool->StartThreadPool();
}

void Application::StartNetwork(int test_code)
{
	if (test_code == 1)
	{
		// m_server_net->StartNetwork(3010);
		m_server_net->StartNetwork(3010, 30);
	}
	else
	{
		m_server_net->StartNetwork(3020, 30);
	}
	
}

void Application::StartNetEventLoop()
{
	m_server_net->EventLoop();
}