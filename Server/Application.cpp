#include "Application.h"

#include "ThreadRouter.h"
#include "MasterObj.h"

Application::Application() : 
	m_thread_pool(std::make_unique<ThreadPool>()),
	// m_server_net(std::make_unique<ServerNet<ClientNet>>()),
	m_server_net(std::make_unique<ServerNet>()),
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

void Application::LoadConfig()
{
	// load config
}

void Application::StartLoadAllLibrary(int test_code)
{
	m_plugin_manager->LoadAllPluginLibrary(test_code);
}

//

void Application::LibInit()
{
	m_plugin_manager->Init();
}

void Application::LibAfterInit()
{
	m_plugin_manager->AfterInit();
}

void Application::LibReadyExecute()
{
	m_plugin_manager->ReadyExecute();
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
		m_server_net->StartNetwork("192.168.169.132", 3010, 30);
	}
	else
	{
		m_server_net->StartNetwork("192.168.169.132", 3020, 30);
	}
	
}

void Application::StartNetEventLoop()
{
	m_server_net->EventLoop();
}

void Application::ConnectMaster()
{
	// Connect Master
	MasterObj* ptr_master_obj = new MasterObj();
	ptr_master_obj->ConnectMaster();
	m_server_net->AddFD(ptr_master_obj);
}