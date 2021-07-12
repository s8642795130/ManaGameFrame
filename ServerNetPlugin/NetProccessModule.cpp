#include "NetProccessModule.h"
#include "BackendClient.h"
#include "../Server/BuiltInMsgDefine.h"
#include "../Server/BuiltInDataDefine.h"
#include "../Server/UnpackNetMsg.h"
#include "../Server/PackageNetMsg.h"
#include "../ActorPlugin/ActorMsg.h"

void NetProccessModule::Init()
{
	m_router_module = m_ptr_manager->GetModule<IMsgRouterModule>();
	m_callback_module = m_ptr_manager->GetModule<INetCallbackModule>();
	m_server_obj_module = m_ptr_manager->GetModule<IServerObjModule>();
	m_config_module = m_ptr_manager->GetModule<IConfigModule>();
	m_client_net_module = m_ptr_manager->GetModule<IClientNetModule>();
	m_thread_pool_module = m_ptr_manager->GetModule<IThreadPoolModule>();
}

/*

void NetProccessModule::ProcessFrontendUnknowMsg(std::shared_ptr<IClientNetActor> client)
{
	// check login msg
	if (client->GetBuffer()->GetMajorId() != static_cast<int>(BuiltInMsg::ServerMsg::LOGIN_MSG)) // LOGIN_MSG
	{
		// check
		client->SetUid("");
		m_client_net_module->AddLoginClientToMap(client);
	}
	else if (client->GetBuffer()->GetMajorId() != static_cast<int>(BuiltInMsg::ServerMsg::SERVER_ONLINE))
	{
		// check server online
		ConnectServerOnline connect_server_online;
		UnpackStructForEachField(connect_server_online, client->GetBuffer());

		// get server data by server_name
		auto server_type = m_config_module->GetTypeByServerName(connect_server_online.m_server_name);
		switch (server_type)
		{
		case EnumDefine::ServerType::FRONTEND:
			client->SetClientType(EnumDefine::ClientType::FRONTEND);
			break;
		case EnumDefine::ServerType::BACKEND:
			client->SetClientType(EnumDefine::ClientType::BACKEND);
			break;
		default:
			std::perror("frontend unknow msg error: error server type");
			break;
		}
		
		// save server to map
		m_server_obj_module->SaveServerToMap(connect_server_online.m_server_name, client->GetUUID());
	}
	else
	{
		// error...
		std::perror("ProcessFrontendUnknowMsg: error");
	}
}

void NetProccessModule::ProcessTempIO(std::shared_ptr<IClientNetActor> client)
{
	int majorId = client->GetBuffer()->GetMajorId();
	auto map_callback = m_callback_module->GetReceiveCallbackMap();

	// check
	if (map_callback.find(majorId) != std::cend(map_callback))
	{
		auto callback = map_callback[majorId];
		callback->Trigger(m_thread_pool_module, client);
	}
}

void NetProccessModule::ProcessHttpIO(IClientNetActor& client)
{
	int majorId = client.GetBuffer()->GetMajorId();
	auto map_callback = m_callback_module->GetHTTPCallbackMap();

	// check
	if (map_callback.find(majorId) != std::cend(map_callback))
	{
		auto callback = map_callback[majorId];
		callback->Trigger(m_thread_pool_module, client);
	}
}
*/