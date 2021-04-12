#include <functional>

#include "MasterModule.h"
#include "../Server/MessageDefine.h"


void MasterModule::Init()
{
	// load actor
	m_master_actor = std::make_shared<MasterActor>();
}

void MasterModule::AfterInit()
{
	// m_plugin->
	int e = static_cast<std::underlying_type_t<NetMessage::ServerMsg>>(NetMessage::ServerMsg::SERVER_ONLINE);
	std::function<void(ClientDescriptor*, int, char*)> call_func = std::bind(&MasterModule::OnServerOnlineCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_plugin->GetAppPtr()->AddReceiveCallBack(e, call_func);
}

void MasterModule::ReadyExecute()
{

}

void MasterModule::Execute()
{

}

//

void MasterModule::OnServerOnlineCallback(ClientDescriptor* ptr_client, int length, char* value)
{

}