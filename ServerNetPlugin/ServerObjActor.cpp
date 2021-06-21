#include "ServerObjActor.h"
#include "../Server/CommonStruct.h"
#include "../Server/BuiltInDataDefine.h"
#include "../Server/BuiltInMsgDefine.h"
#include "../Server/PackageNetMsg.h"
#include "../Server/UnpackNetMsg.h"

void ServerObjActor::OnServerOnlineCallback(IClientNetActor& ptr_client)
{
	// buffer
	auto buffer = ptr_client.GetBuffer();

	// unpack
	ServerOnlineInfo server_online_info;
	UnpackStructForEachField(server_online_info, buffer);

	std::for_each(std::cbegin(server_online_info.m_vec_server), std::cend(server_online_info.m_vec_server), [this](const ServerOnlineData& item) -> void
		{
			/*
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
			*/
		}
	);
}