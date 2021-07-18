#include "BackendServerActor.h"
#include "BackendClient.h"
#include "../Server/StringDefine.h"
#include "../Server/UnpackNetMsg.h"
#include "../Server/BuiltInMsgDefine.h"

void BackendServerActor::SetClientType(EnumDefine::ClientType client_type)
{
	m_client_type = client_type;
}

void BackendServerActor::ProcessIO()
{
	switch (m_client_type)
	{
	case EnumDefine::ClientType::FRONTEND:
		ProcessServerBackendIO();
		break;
	case EnumDefine::ClientType::BACKEND:
		// backend server
		ProcessRPCIO();
		break;
	default:
		ProcessServerOnline();
		break;
	}
}

void BackendServerActor::ProcessServerBackendIO()
{
	// parsing
	FrontendToBackendMsg backend_msg;
	UnpackStructForEachField(backend_msg, m_buffer);

	// buffer
	std::unique_ptr<ByteBuffer> buffer{ std::make_unique<ByteBuffer>() };

	// call callback
	auto map_callback = m_client_impl->m_callback_module->GetBackendCallbackMap();
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

void BackendServerActor::ProcessRPCIO()
{
	// parsing
	RPCMsgData rpc_data;
	UnpackStructForEachField(rpc_data, m_buffer);

	// map callback
	auto map_callback = m_client_impl->m_callback_module->GetRPCCallbackMap();

	// find callback
	if (map_callback.find(rpc_data.m_major_id) != std::cend(map_callback))
	{
		map_callback[rpc_data.m_major_id](rpc_data.m_major_id, rpc_data.m_minor_id, rpc_data.m_stream);
	}
}

void BackendServerActor::ProcessServerOnline()
{
	// check login msg
	if (m_buffer->GetMajorId() != static_cast<int>(BuiltInMsg::ServerMsg::SERVER_ONLINE))
	{
		// check server online
		ConnectServerOnline connect_server_online;
		UnpackStructForEachField(connect_server_online, m_buffer);

		// get server data by server_name
		auto server_type = m_client_impl->m_config_module->GetTypeByServerName(connect_server_online.m_server_name);
		switch (server_type)
		{
		case EnumDefine::ServerType::FRONTEND:
			SetClientType(EnumDefine::ClientType::FRONTEND);
			break;
		case EnumDefine::ServerType::BACKEND:
			SetClientType(EnumDefine::ClientType::BACKEND);
			break;
		default:
			std::perror("frontend unknow msg error: error server type");
			break;
		}

		// save server to map
		// m_server_obj_module->SaveServerToMap(connect_server_online.m_server_name, client.GetUUID());
	}
	else
	{
		// error...
		std::cout << BACKEND_PROCESS_ERROR << std::endl;
	}
}