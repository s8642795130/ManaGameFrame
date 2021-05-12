#include <functional>

#include "MasterModule.h"
#include "../Server/MessageDefine.h"
#include "../Server/ActorMsg.h"

void MasterModule::Init()
{
	// load actor
	m_master_actor = std::make_shared<MasterActor>();
}

void MasterModule::AfterInit()
{
	std::function<void(ClientDescriptor*)> call_func = std::bind(&MasterModule::OnServerOnlineCallback, this, std::placeholders::_1);
	// auto call_func = std::bind(&MasterModule::OnServerOnlineCallback);
	m_plugin->GetAppPtr()->AddReceiveCallBack(static_cast<int>(NetMessage::ServerMsg::SERVER_ONLINE), call_func);
}

//

void MasterModule::OnServerOnlineCallback(ClientDescriptor* ptr_client)
{
	// notify other servers that a server online
	std::unique_ptr<IActorMsg> ptr = std::make_unique<ActorMsg<void, MasterActor, int, int>>("", m_master_actor->GetUUID(), &MasterActor::ServerOnline, 10, 20);
	m_plugin->GetAppPtr()->SendMsgToActor(ptr);
}