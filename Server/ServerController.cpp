#include <algorithm>

#include "ServerController.h"
#include "ConfigFile.h"
#include "ServerObj.h"

struct OnlineServerData
{
public:
	std::string m_server_type;
	std::string m_server_name;
	std::string m_server_ip;
	int m_port;
};

void ServerController::SaveServerUUID(const std::string& server_name, const std::string& uuid)
{
	m_map_server.emplace(server_name, uuid);
}

void ServerController::ConnectMaster()
{
	// get master server ip and port
	auto vec_server_name = m_ptr_config_file->GetServersByType("master");
	auto server_data = m_ptr_config_file->GetServerDataByName(vec_server_name[0]);

	std::shared_ptr<ServerObj> server_obj{ std::make_shared<ServerObj>() };
	SaveServerUUID(vec_server_name[0], server_obj->GetUUID());

	while (true)
	{
		// connect master
		if (false == server_obj->ConnectServer(server_data->m_server_ip, server_data->m_port))
		{
			break;
		}

		// send this server is online
		server_obj->SendData(static_cast<int>(NetMessage::ServerMsg::SERVER_ONLINE), 0, "", 0);

		// recv already online server list
		server_obj->RecvData();
	}
	
}

void ServerController::ConnectServer()
{
	// first connect master server
	ConnectMaster();

	std::vector<OnlineServerData> vec_online_server;

	std::for_each(std::cbegin(vec_online_server), std::cend(vec_online_server), [this](const OnlineServerData& item) -> void
		{
			std::shared_ptr<ServerObj> server_obj{ std::make_shared<ServerObj>() };
			SaveServerUUID(item.m_server_name, server_obj->GetUUID());

			//
			server_obj->ConnectServer(item.m_server_ip, item.m_port);

			//
			server_obj->SendData(static_cast<int>(NetMessage::ServerMsg::SERVER_ONLINE), 0, "", 0);
		}
	);
}