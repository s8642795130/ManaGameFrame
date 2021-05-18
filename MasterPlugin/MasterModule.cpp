#include <functional>

#include "MasterModule.h"
#include "../Server/MessageDefine.h"
#include "../Server/ActorMsg.h"
#include "../Server/MessageData.h"
#include "../Server/GameMessageData.h"

void MasterModule::Init()
{
	// load actor
	m_master_actor = std::make_shared<MasterActor>();
}

void MasterModule::AfterInit()
{
	std::function<void(ClientDescriptor*)> call_func = std::bind(&MasterModule::OnServerOnlineCallback, this, std::placeholders::_1);
	m_plugin->GetAppPtr()->AddReceiveCallBack(static_cast<int>(NetMessage::ServerMsg::SERVER_ONLINE), call_func);
}

//

void MasterModule::OnServerOnlineCallback(ClientDescriptor* ptr_client)
{
	// notify other servers that a server online
	ptr_client->m_buffer->SetPosToDataBegin();
	ConnectServerOnline connect_server_online;
	ForEachField(connect_server_online, ptr_client->m_buffer);

	std::cout << "recv server data !!!" << connect_server_online.m_server_name << std::endl;

	std::unique_ptr<IActorMsg> ptr = std::make_unique<ActorMsg<void, MasterActor, int, int>>("", m_master_actor->GetUUID(), &MasterActor::ServerOnline, 10, 20);
	m_plugin->GetAppPtr()->SendMsgToActor(ptr);
}