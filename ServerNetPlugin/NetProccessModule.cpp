#include "NetProccessModule.h"
#include "IClientNetActor.h"
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
void NetProccessModule::ProcessBackendIO(IClientNetActor& client)
{
	// there is two situations, case 1: back msg to client; case 2: change client data
	if (client.GetBuffer()->GetMajorId() != static_cast<int>(BuiltInMsg::ServerMsg::RETURN_CLIENT_MSG))
	{
		// unpack msg
		BackendMsgToClient backend_msg;
		UnpackStructForEachField(backend_msg, client.GetBuffer());

		// find login uid
		auto ptr_client = m_client_net_module->GetLoginClient(backend_msg.m_client_uid);

		if (ptr_client != nullptr)
		{
			// send to client
			std::unique_ptr<IActorMsg> ptr = CreateActorMsg(client.GetUUID(), ptr_client->GetUUID(), &IClientNetActor::BackStream, std::move(backend_msg.m_buffer));
			client.GetActorPimpl()->SendMsgToActor(ptr);
		}
	}
	else if (client.GetBuffer()->GetMajorId() != static_cast<int>(BuiltInMsg::ServerMsg::UPDATE_CLIENT_DATA))
	{
		// unpack msg
		UpdateClient update_client;
		UnpackStructForEachField(update_client, client.GetBuffer());

		// find client uid
		auto ptr_client = m_client_net_module->GetLoginClient(update_client.m_client_uid);
		if (ptr_client != nullptr)
		{
			ptr_client->UpdateClientData(update_client.m_data_key, update_client.m_data_value, update_client.m_update_type);
		}
	}
	else
	{
		// error...
		std::perror("ProcessBackendIO: error");
	}
}

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

void NetProccessModule::ProcessBackendUnknowMsg(IClientNetActor& client)
{
	// check login msg
	if (client.GetBuffer()->GetMajorId() != static_cast<int>(BuiltInMsg::ServerMsg::SERVER_ONLINE))
	{
		// check server online
		ConnectServerOnline connect_server_online;
		UnpackStructForEachField(connect_server_online, client.GetBuffer());

		// get server data by server_name
		auto server_type = m_config_module->GetTypeByServerName(connect_server_online.m_server_name);
		switch (server_type)
		{
		case EnumDefine::ServerType::FRONTEND:
			client.SetClientType(EnumDefine::ClientType::FRONTEND);
			break;
		case EnumDefine::ServerType::BACKEND:
			client.SetClientType(EnumDefine::ClientType::BACKEND);
			break;
		default:
			std::perror("frontend unknow msg error: error server type");
			break;
		}

		// save server to map
		m_server_obj_module->SaveServerToMap(connect_server_online.m_server_name, client.GetUUID());
	}
	else
	{
		// error...
	}
}

void NetProccessModule::ProcessServerBackendIO(IClientNetActor& client)
{
	// parsing
	FrontendToBackendMsg backend_msg;
	UnpackStructForEachField(backend_msg, client.GetBuffer());

	// buffer
	std::unique_ptr<ByteBuffer> buffer{ std::make_unique<ByteBuffer>() };

	// call callback
	auto map_callback = m_callback_module->GetBackendCallbackMap();
	if (map_callback.find(buffer->GetMajorId()) != std::cend(map_callback))
	{
		// create param
		BackendClient backend_client(std::move(buffer));
		backend_client.m_uid = backend_msg.m_client_uid;
		backend_client.m_client_data = backend_msg.m_client_data;

		// call callback
		auto callback = map_callback[buffer->GetMajorId()];
		callback(backend_client);
	}
}

void NetProccessModule::ProcessRPCIO(IClientNetActor& client)
{
	// parsing
	RPCMsgData rpc_data;
	UnpackStructForEachField(rpc_data, client.GetBuffer());

	// map callback
	auto map_callback = m_callback_module->GetRPCCallbackMap();

	// find callback
	if (map_callback.find(rpc_data.m_major_id) != std::cend(map_callback))
	{
		map_callback[rpc_data.m_major_id](rpc_data.m_major_id, rpc_data.m_minor_id, rpc_data.m_stream);
	}
}

void NetProccessModule::ProcessMasterIO(IClientNetActor& client)
{
	// data
	int majorId = client.GetBuffer()->GetMajorId();
	auto map_callback = m_callback_module->GetMasterCallbackMap();

	// check
	if (map_callback.find(majorId) != std::cend(map_callback))
	{
		auto callback = map_callback[majorId];
		callback(client);
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