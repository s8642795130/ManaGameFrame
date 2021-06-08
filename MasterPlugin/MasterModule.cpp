#include <functional>

#include "MasterModule.h"
#include "../Server/BuiltInMsgDefine.h"
#include "../Server/BuiltInDataDefine.h"
#include "../Server/UnpackNetMsg.h"
#include "../ActorPlugin/ActorMsg.h"

void MasterModule::Init()
{
	// module
	m_callback_module = m_ptr_manager->GetModule<INetCallbackModule>();
	m_thread_pool_module = m_ptr_manager->GetModule<IThreadPoolModule>();

	// load actor
	m_master_actor = std::make_shared<MasterActor>(m_ptr_manager);
}

void MasterModule::AfterInit()
{
	// add actor to thread poll
	m_thread_pool_module->AddActorToThreadCell(m_master_actor);

	// bind msg
	std::function<void(IClientNetActor&)> call_func = std::bind(&MasterModule::OnServerOnlineCallback, this, std::placeholders::_1);
	m_callback_module->AddReceiveCallBack(static_cast<int>(BuiltInMsg::ServerMsg::SERVER_ONLINE), call_func);
}

//

void MasterModule::OnServerOnlineCallback(IClientNetActor& client)
{
	// notify other servers that a server online
	ConnectServerOnline connect_server_online;
	UnpackStructForEachField(connect_server_online, client.GetBuffer());

	std::unique_ptr<IActorMsg> ptr = std::make_unique<ActorMsg<void, MasterActor, const std::string, const std::string>>("", m_master_actor->GetUUID(), &MasterActor::ServerOnline, connect_server_online.m_server_name, client.GetUUID());
	m_thread_pool_module->AddActorMsgToThreadCell(ptr);
}