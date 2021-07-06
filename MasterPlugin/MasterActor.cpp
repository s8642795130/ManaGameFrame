#include <iostream>

#include "MasterActor.h"
#include "../Server/PackageNetMsg.h"
#include "../Server/IConfigFile.h"
#include "../Server/BuiltInDataDefine.h"
#include "../ActorPlugin/ActorMsg.h"
#include "../Server/UnpackNetMsg.h"

void MasterActor::OnServerOnlineCallback(std::shared_ptr<INetActor> ptr_client)
{
	// notify other servers that a server online
	ConnectServerOnline connect_server_online;
	UnpackStructForEachField(connect_server_online, ptr_client->GetBuffer());

	ServerOnline(connect_server_online.m_server_name, ptr_client->GetUUID());
}

void MasterActor::ServerOnline(const std::string server_name, const std::string uuid)
{
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

	// send online server struct to server
	std::unique_ptr<IActorMsg> ptr = CreateActorMsg(GetUUID(), uuid, &IClientNetActor::SendStream, std::move(buffer));
	m_pimpl->SendMsgToActor(ptr);
}