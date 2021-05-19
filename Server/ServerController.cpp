#include <algorithm>

#include "ServerController.h"
#include "ConfigFile.h"
#include "IThreadPool.h"
#include "ServerObj.h"
#include "MessageData.h"
#include "PackageNetMsg.h"
#include "GameMessageData.h"

void ServerController::SaveServerUUID(const std::string& server_name, const std::string& uuid)
{
	m_map_server.emplace(server_name, uuid);
}

void ServerController::ConnectMaster()
{
	// get master server ip and port
	auto vec_server_name = m_ptr_config_file->GetServersByType("master");
	auto server_data = m_ptr_config_file->GetServerDataByName(vec_server_name[0]->m_server_name);

	// create a server obj
	std::shared_ptr<ServerObj> server_obj{ std::make_shared<ServerObj>() };

	// save this server obj
	SaveServerUUID(vec_server_name[0]->m_server_name, server_obj->GetUUID());
	m_ptr_thread_pool->AddActorToThreadCell(server_obj);

	// connect server and send SERVER_ONLINE
	ServerOnlineInfo server_online_info;
	std::shared_ptr<ByteBuffer> buffer;
	while (true)
	{
		// connect master
		if (false == server_obj->ConnectServer(server_data->m_server_ip, server_data->m_port))
		{
			break;
		}

		// send this server is online
		ConnectServerOnline connect_server_online = { 0 };
		connect_server_online.m_server_name = m_ptr_config_file->GetMyServerInfo()->m_server_name;
		std::vector<char> msg_buffer;
		PackageStructForEachField(connect_server_online, msg_buffer);
		server_obj->SendData(static_cast<int>(NetMessage::ServerMsg::SERVER_ONLINE), 0, msg_buffer.data(), static_cast<int>(msg_buffer.size()));

		// recv already online server list
		buffer = server_obj->RecvData();
		
		// ServerTest server_online_info;
		ForEachField(server_online_info, buffer);

		// exit while
		break;
	}

	std::for_each(std::cbegin(server_online_info.m_vec_server), std::cend(server_online_info.m_vec_server), [this](const ServerOnlineData& item) -> void
		{
			std::shared_ptr<ServerObj> server_obj{ std::make_shared<ServerObj>() };
			SaveServerUUID(item.m_server_name, server_obj->GetUUID());

			//
			server_obj->ConnectServer(item.m_ip, item.m_port);

			//
			ConnectServerOnline connect_server_online = { 0 };
			std::vector<char> buffer;
			PackageStructForEachField(connect_server_online, buffer);
			server_obj->SendData(static_cast<int>(NetMessage::ServerMsg::SERVER_ONLINE), 0, buffer.data(), static_cast<int>(buffer.size()));
		}
	);
}

void ServerController::ConnectServer()
{
	// first connect master server
	ConnectMaster();
}