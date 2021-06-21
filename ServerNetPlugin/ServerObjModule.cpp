#include "ServerObjModule.h"
#include "../Server/CommonStruct.h"
#include "../Server/BuiltInDataDefine.h"
#include "../Server/BuiltInMsgDefine.h"
#include "../Server/PackageNetMsg.h"
#include "../Server/UnpackNetMsg.h"
#include "BindFunc.h"

void ServerObjModule::Init()
{
	m_client_net_module = m_ptr_manager->GetModule<IClientNetModule>();
	m_config_module = m_ptr_manager->GetModule<IConfigModule>();
	m_server_net_module = m_ptr_manager->GetModule<IServerNetModule>();
	m_callback_module = m_ptr_manager->GetModule<INetCallbackModule>();
	// actor
	m_server_obj_actor = std::make_shared<ServerObjActor>(m_ptr_manager);
}

void ServerObjModule::AfterInit()
{
	if (m_config_module->GetServerType() == EnumDefine::ServerType::MASTER)
	{
		return;
	}

	// bind msg
	//std::function<void(IClientNetActor&)> call_func = std::bind(&ServerObjModule::OnServerOnlineCallback, this, std::placeholders::_1);
	auto call_func = std::make_shared<BindFunc<ServerObjActor>>(static_cast<int>(BuiltInMsg::ServerMsg::SERVER_ONLINE), m_server_obj_actor->GetUUID(), &ServerObjActor::OnServerOnlineCallback);
	m_callback_module->AddReceiveCallback(static_cast<int>(BuiltInMsg::ServerMsg::SERVER_ONLINE), call_func);

	// create client obj
	std::shared_ptr<IClientNetActor> ptr_client = m_client_net_module->CreateClientNet();

	// get master config
	auto vec_server_name = m_config_module->GetServersByType("master");
	auto server_data = m_config_module->GetServerDataByName(vec_server_name[0]->m_server_name);

	// save this server obj
	SaveServerToMap(vec_server_name[0]->m_server_name, ptr_client->GetUUID());

	// connect server and send SERVER_ONLINE
	ServerOnlineInfo server_online_info;
	std::shared_ptr<ByteBuffer> buffer;

	//
	while (true)
	{
		// connect master
		if (false == ptr_client->ConnectServer(server_data->m_server_ip, server_data->m_port))
		{
			break;
		}

		// add to server fd
		m_server_net_module->AddFD(ptr_client);

		// send this server is online
		ConnectServerOnline connect_server_online;
		connect_server_online.m_server_name = m_config_module->GetMyServerInfo()->m_server_name;

		// package
		std::vector<char> msg_buffer;
		PackageStructForEachField(connect_server_online, msg_buffer);
		ptr_client->SendData(static_cast<int>(BuiltInMsg::ServerMsg::SERVER_ONLINE), 0, msg_buffer);

		// exit while
		break;
	}
}

void ServerObjModule::SaveServerToMap(const std::string& server_name, const std::string& uuid)
{
	m_map_server.emplace(server_name, uuid);
}

// interface

const std::string ServerObjModule::GetServerUUIDByName(const std::string& server_name)
{
	std::string uuid;
	if (m_map_server.find(server_name) != std::cend(m_map_server))
	{
		uuid = m_map_server[server_name];
	}
	return uuid;
}