#include "ServerObjModule.h"
#include "../Server/CommonStruct.h"
#include "../Server/BuiltInDataDefine.h"
#include "../Server/BuiltInMsgDefine.h"
#include "../Server/PackageNetMsg.h"
#include "../Server/UnpackNetMsg.h"

void ServerObjModule::Init()
{
	m_client_net_module = m_ptr_manager->GetModule<IClientNetModule>();
	m_config_module = m_ptr_manager->GetModule<IConfigModule>();
	m_server_net_module = m_ptr_manager->GetModule<IServerNetModule>();
}

void ServerObjModule::AfterInit()
{
	if (m_config_module->GetServerType() == EnumDefine::ServerType::MASTER)
	{
		return;
	}

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

// callback

void ServerObjModule::OnServerOnlineCallback(IClientNetActor& ptr_client)
{
	// buffer
	auto buffer = ptr_client.GetBuffer();

	// unpack
	ServerOnlineInfo server_online_info;
	UnpackStructForEachField(server_online_info, buffer);

	std::for_each(std::cbegin(server_online_info.m_vec_server), std::cend(server_online_info.m_vec_server), [this](const ServerOnlineData& item) -> void
		{
			std::shared_ptr<IClientNetActor> ptr_client = m_client_net_module->CreateClientNet();
			SaveServerToMap(item.m_server_name, ptr_client->GetUUID());

			// get server data
			auto server_data = m_config_module->GetServerDataByName(item.m_server_name);

			if (!(server_data->m_server_type.compare("connector") && m_config_module->GetServerType() == EnumDefine::ServerType::FRONTEND))
			{
				// connect server
				ptr_client->ConnectServer(server_data->m_server_ip, server_data->m_port);

				// struct msg
				ConnectServerOnline connect_server_online;
				connect_server_online.m_server_name = m_config_module->GetMyServerInfo()->m_server_name;

				// package
				std::vector<char> buffer;
				PackageStructForEachField(connect_server_online, buffer);
				ptr_client->SendData(static_cast<int>(BuiltInMsg::ServerMsg::SERVER_ONLINE), 0, buffer);
			}
		}
	);
}