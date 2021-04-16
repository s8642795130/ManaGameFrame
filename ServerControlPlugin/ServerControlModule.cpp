#include "ServerControlModule.h"

#include "../Server/MessageDefine.h"

void ServerControlModule::Init()
{
	// load actor
	// m_master_actor = std::make_shared<MasterActor>();
}

void ServerControlModule::AfterInit()
{
	int e = static_cast<std::underlying_type_t<NetMessage::ServerMsg>>(NetMessage::ServerMsg::SERVER_ONLINE);
	std::function<void(ClientDescriptor*, int, char*)> call_func = std::bind(&ServerControlModule::OnServerOnlineCallback, this, std::placeholders::_1);
	
	m_plugin->GetAppPtr()->AddReceiveCallBack(e, call_func);
}

void ServerControlModule::ReadyExecute()
{

}

void ServerControlModule::Execute()
{

}

void ServerControlModule::OnServerOnlineCallback(ClientDescriptor* ptr_client)
{
	// struct
}