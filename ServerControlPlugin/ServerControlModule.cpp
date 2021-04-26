#include "ServerControlModule.h"

#include "../Server/MessageDefine.h"
#include "../Server/ActorMsg.h"

void ServerControlModule::Init()
{
	// load actor
	// m_master_actor = std::make_shared<MasterActor>();
}

void ServerControlModule::AfterInit()
{
	int e = static_cast<std::underlying_type_t<NetMessage::ServerMsg>>(NetMessage::ServerMsg::SERVER_ONLINE);
	std::function<void(ClientDescriptor*)> call_func = std::bind(&ServerControlModule::OnServerOnlineCallback, this, std::placeholders::_1);
	
	m_plugin->GetAppPtr()->AddReceiveCallBack(e, call_func);

	int frontend_msg = static_cast<std::underlying_type_t<NetMessage::ServerMsg>>(NetMessage::ServerMsg::FRONTEND_MSG);
	std::function<void(ClientDescriptor*)> frontend_msg_call_func = std::bind(&ServerControlModule::OnFrontendMsgCallback, this, std::placeholders::_1);

	m_plugin->GetAppPtr()->AddReceiveCallBack(frontend_msg, frontend_msg_call_func);
}

void ServerControlModule::Execute()
{
	ConnectMaster();
}

// function

void ServerControlModule::ConnectMaster()
{
	//
	std::shared_ptr<ServerObj> server_obj = std::make_shared<ServerObj>();
	server_obj->ConnectServer("127.0.0.1", 3010);
	m_plugin->GetAppPtr()->AddActorToThreadCell(server_obj);

	//
	const int server_online = static_cast<std::underlying_type_t<NetMessage::ServerMsg>>(NetMessage::ServerMsg::SERVER_ONLINE);
	std::unique_ptr<IActorMsg> ptr_actor_msg = std::make_unique<ActorMsg<void, ClientDescriptor, const int, const int, const char*, const int>>("", server_obj->GetUUID(), &ClientDescriptor::SendData, server_online, 0, nullptr, 0);
	m_plugin->GetAppPtr()->SendMsgToActor(ptr_actor_msg);

	//
	m_map_server_obj.emplace(server_obj->GetUUID(), server_obj);
}

// callback

void ServerControlModule::OnServerOnlineCallback(ClientDescriptor* ptr_client)
{
	// master told this module that the other server is online
}

void ServerControlModule::OnServerConnectCallback(ClientDescriptor* ptr_client)
{
	// other server is already connect this server, record that
}

void ServerControlModule::OnFrontendMsgCallback(ClientDescriptor* ptr_client)
{
	// the frontend server received some message from the client
}