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
}

void NetProccessModule::ProcessFrontendIO(IClientNetActor& client)
{
	// send to backend
	//if (!m_is_login)
	//{
		//return;
	//}

	// get msg corresponding to plugin
	const auto majorId = client.GetBuffer()->GetMajorId();
	auto map_msg = m_callback_module->GetGameMsgMap();
	auto plugin_name = map_msg[majorId];

	// get all the servers that the plugin exists
	const auto server_list = m_config_module->GetServersByPluginName(plugin_name);

	// router
	const auto server_index = m_router_module->GetMsgRouterByClient(plugin_name, static_cast<int>(server_list.size()), client);

	// get server uuid
	auto server_uuid = m_server_obj_module->GetServerUUIDByName(server_list[server_index]->m_server_name);

	// create backend client struct
	FrontendToBackendMsg backend_msg;
	auto buffer = client.GetBuffer();
	backend_msg.m_client_uid = client.GetUid();
	backend_msg.m_client_uuid = client.GetUUID();
	backend_msg.m_client_data = client.GetClientData();
	backend_msg.m_major_id = buffer->GetMajorId();
	backend_msg.m_minor_id = buffer->GetMinorId();
	backend_msg.m_stream = buffer->GetStream();

	// packge
	std::vector<char> package;
	PackageStructForEachField(backend_msg, package);

	// send to backend server
	std::unique_ptr<IActorMsg> ptr = std::make_unique<ActorMsg<void, IClientNetActor, std::vector<char>>>(client.GetUUID(), server_uuid, &IClientNetActor::SendStream, std::move(package));
	client.GetActorPimpl()->SendMsgToActor(ptr);
}

void NetProccessModule::ProcessBackendIO(IClientNetActor& client)
{
	// there is two situations, case 1: back msg to client; case 2: change client data
	if (client.GetBuffer()->GetMajorId() != static_cast<int>(BuiltInMsg::ServerMsg::RETURN_CLIENT_MSG))
	{
		// unpack msg
		BackendMsgToClient backend_msg;
		UnpackStructForEachField(backend_msg, client.GetBuffer());

		// send to client
		std::unique_ptr<IActorMsg> ptr = std::make_unique<ActorMsg<void, IClientNetActor, std::vector<char>>>(client.GetUUID(), backend_msg.m_client_uuid, &IClientNetActor::SendStream, std::move(backend_msg.m_buffer));
		client.GetActorPimpl()->SendMsgToActor(ptr);
	}
	else if (client.GetBuffer()->GetMajorId() != static_cast<int>(BuiltInMsg::ServerMsg::UPDATE_CLIENT_DATA))
	{

	}
	else
	{
		// error...
	}
}

void NetProccessModule::ProcessFrontendUnknowMsg(IClientNetActor& client)
{
	// check login msg
	if (client.GetBuffer()->GetMajorId() != static_cast<int>(BuiltInMsg::ServerMsg::LOGIN_MSG)) // LOGIN_MSG
	{
		// check

		//
		return;
	}
	else if (1)
	{
		// check server online
	}
}

void NetProccessModule::ProcessServerBackendIO(IClientNetActor& client)
{
	// parsing
	FrontendToBackendMsg backend_msg;
	UnpackStructForEachField(backend_msg, client.GetBuffer());

	// buffer
	std::unique_ptr<ByteBuffer> buffer{ std::make_unique<ByteBuffer>() };

	// create param
	BackendClient backend_client(std::move(buffer));
	backend_client.m_uid = backend_msg.m_client_uid;
	backend_client.m_uuid = backend_msg.m_client_uuid;
	backend_client.m_client_data = backend_msg.m_client_data;

	// call callback
	auto map_callback = m_callback_module->GetBackendCallBackMap();
	if (map_callback.find(buffer->GetMajorId()) != std::cend(map_callback))
	{
		auto callback = map_callback[buffer->GetMajorId()];
		callback(backend_client);
	}
}

void NetProccessModule::ProcessRPCIO(IClientNetActor& client)
{

}

void NetProccessModule::ProcessMasterIO(IClientNetActor& client)
{
	// data
	int majorId = client.GetBuffer()->GetMajorId();
	auto map_callback = m_callback_module->GetReceiveCallBackMap();

	// check
	if (map_callback.find(majorId) != std::cend(map_callback))
	{
		std::function<void(IClientNetActor&)> callback = map_callback[majorId];
		callback(client);
	}
}