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
	int e = static_cast<std::underlying_type_t<NetMessage::ServerMsg>>(NetMessage::ServerMsg::SERVER_ONLINE);
	std::function<void(ClientDescriptor*)> call_func = std::bind(&MasterModule::OnServerOnlineCallback, this, std::placeholders::_1);
	m_plugin->GetAppPtr()->AddReceiveCallBack(e, call_func);
}

//

void MasterModule::OnServerOnlineCallback(ClientDescriptor* ptr_client)
{
	// notify other servers that a server online
	std::unique_ptr<IActorMsg> ptr = std::make_unique<ActorMsg<void, MasterActor>>("", m_master_actor->GetUUID(), &MasterActor::ServerOnline);
	this->m_plugin->GetAppPtr()->SendMsgToActor(ptr);
}