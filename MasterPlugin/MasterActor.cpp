#include <iostream>

#include "MasterActor.h"
#include "../Server/PackageNetMsg.h"
#include "../Server/IConfigFile.h"
#include "../Server/BuiltInDataDefine.h"

std::shared_ptr<IActorPimpl> IActorPimpl::m_pimpl;

void MasterActor::ServerOnline(const std::string server_name, const std::string uuid)
{
	std::cout << "Server is online !" << std::endl;

	ServerOnlineInfo server_online_info;
	for (auto& it : m_map_online_server)
	{
		ServerOnlineData server_online_data;
		server_online_data.m_server_name = it.first;

		//
		server_online_info.m_vec_server.push_back(server_online_data);
	}

	// package message
	std::vector<char> buffer;
	PackageStructForEachField(server_online_info, buffer);

	// save online server data
	m_map_online_server.emplace(server_name, uuid);
}