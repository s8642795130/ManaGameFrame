#include <iostream>

#include "MasterActor.h"
#include "../Server/StringDefine.h"
#include "../Server/PackageNetMsg.h"
#include "../Server/IConfigFile.h"
#include "../Server/BuiltInDataDefine.h"
#include "../Server/UnpackNetMsg.h"
#include "../Server/BuiltInMsgDefine.h"
#include "../ActorPlugin/ActorMsg.h"

void MasterActor::OnServerOnlineCallback(std::shared_ptr<INetActor> ptr_client)
{
	// notify other servers that a server online
	ConnectServerOnline connect_server_online;
	UnpackStructForEachField(connect_server_online, ptr_client->GetBuffer());

	std::cout << connect_server_online.m_server_name << IS_ONLINE << std::endl;

	ServerOnline(connect_server_online.m_server_name, ptr_client->GetUUID());
}

void MasterActor::ServerOnline(const std::string& server_name, const std::string& online_server_uuid)
{
	// broadcast server online
	BroadcastServerOnline(server_name);

	// send current online server data to online server
	SendCurrentAllServerToOnlineServer(online_server_uuid);

	// save online server data
	m_map_online_server.emplace(server_name, online_server_uuid);
}

void MasterActor::BroadcastServerOnline(const std::string& server_name)
{
	for (const auto& it : m_map_online_server)
	{
		ServerOnlineData server_online_data;
		server_online_data.m_server_name = server_name;

		std::vector<char> buffer;
		PackageStructForEachField(server_online_data, buffer);

		std::unique_ptr<IActorMsg> ptr = CreateActorMsg(GetUUID(), it.second, &INetActor::SendData, static_cast<int>(BuiltInMsg::ServerMsg::SERVER_ONLINE), 0, std::move(buffer));
		m_pimpl->SendMsgToActor(ptr);
	}
}

void MasterActor::SendCurrentAllServerToOnlineServer(const std::string& online_server_uuid)
{
	ServerOnlineInfo server_online_info;
	for (const auto& it : m_map_online_server)
	{
		ServerOnlineData server_online_data;
		server_online_data.m_server_name = it.first;

		//
		server_online_info.m_vec_server.push_back(server_online_data);
	}

	// package message
	std::vector<char> buffer;
	PackageStructForEachField(server_online_info, buffer);

	// send online server struct to server
	std::unique_ptr<IActorMsg> ptr = CreateActorMsg(GetUUID(), online_server_uuid, &INetActor::SendData, static_cast<int>(BuiltInMsg::ServerMsg::CURRENT_ONLINE_LIST), 0, std::move(buffer));
	m_pimpl->SendMsgToActor(ptr);
}