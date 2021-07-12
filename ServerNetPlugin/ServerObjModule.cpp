#include "ServerObjModule.h"
#include "../Server/CommonStruct.h"
#include "../Server/BuiltInDataDefine.h"
#include "../Server/BuiltInMsgDefine.h"
#include "../Server/PackageNetMsg.h"
#include "../Server/UnpackNetMsg.h"
#include "../Server/StringDefine.h"

void ServerObjModule::Init()
{
	m_client_net_module = m_ptr_manager->GetModule<IClientNetModule>();
	m_config_module = m_ptr_manager->GetModule<IConfigModule>();
	m_server_net_module = m_ptr_manager->GetModule<IServerNetModule>();
	m_callback_module = m_ptr_manager->GetModule<INetCallbackModule>();
	m_poll_module = m_ptr_manager->GetModule<IPollModule>();
}

void ServerObjModule::AfterInit()
{
	if (m_config_module->GetServerType() == EnumDefine::ServerType::MASTER)
	{
		return;
	}

	// bind msg
	auto callback = [this](IPollClient& client) -> void { OnServerOnlineCallback(client); };
	m_callback_module->AddMasterCallback(static_cast<int>(BuiltInMsg::ServerMsg::SERVER_ONLINE), callback);
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

void ServerObjModule::OnServerOnlineCallback(IPollClient& client)
{
	// buffer
	auto buffer = client.GetBuffer();

	// unpack
	ServerOnlineInfo server_online_info;
	UnpackStructForEachField(server_online_info, buffer);

	std::for_each(std::cbegin(server_online_info.m_vec_server), std::cend(server_online_info.m_vec_server), [this](const ServerOnlineData& item) -> void
		{
			// get server data
			auto server_data = m_config_module->GetServerDataByName(item.m_server_name);

			if (!(server_data->m_server_type.compare(STR_CONNECTOR) && m_config_module->GetServerType() == EnumDefine::ServerType::FRONTEND))
			{
				// connect server
				m_poll_module->ConnectServerWithServerName(server_data->m_server_ip, server_data->m_port, item.m_server_name);

				// struct msg
				ConnectServerOnline connect_server_online;
				connect_server_online.m_server_name = m_config_module->GetMyServerInfo()->m_server_name;

				// package
				std::vector<char> buffer;
				PackageStructForEachField(connect_server_online, buffer);

				auto ptr_client = m_poll_module->GetClientByServerName(item.m_server_name);
				ptr_client->SendData(static_cast<int>(BuiltInMsg::ServerMsg::SERVER_ONLINE), 0, buffer);
			}
		}
	);
}