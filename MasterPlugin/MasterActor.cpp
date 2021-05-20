#include <iostream>

#include "MasterActor.h"
#include "../Server/GameMessageData.h"
#include "../Server/PackageNetMsg.h"
#include "../Server/IConfigFile.h"
#include "../Server/CommonStruct.h"

void MasterActor::ServerOnline(const std::string server_name, const std::string uuid)
{
	std::cout << "Server is online !" << std::endl;

	auto config_file = m_app->GetConfigPtr();
	ServerOnlineInfo server_online_info;
	for (auto& it : m_map_online_server)
	{
		auto ptr_server_data = config_file->GetServerDataByName(it.first);
		ServerOnlineData server_online_data;
		server_online_data.m_server_name = ptr_server_data->m_server_name;
		server_online_data.m_server_type = ptr_server_data->m_server_type;
		server_online_data.m_port = ptr_server_data->m_port;
		server_online_data.m_ip = ptr_server_data->m_server_ip;

		//
		server_online_info.m_vec_server.push_back(server_online_data);
	}

	// package message
	std::vector<char> buffer;
	PackageStructForEachField(server_online_info, buffer);

	// save online server data
	m_map_online_server.emplace(server_name, uuid);
}