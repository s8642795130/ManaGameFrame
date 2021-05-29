#include "NetProccessModule.h"
#include "IClientNetActor.h"

void NetProccessModule::Init()
{
	m_router_module = m_ptr_manager->GetModule<IMsgRouterModule>();
}

void NetProccessModule::ProcessFrontendIO(IClientNetActor* client)
{
	// send to backend
	//if (!m_is_login)
	//{
		//return;
	//}

	// get msg corresponding to plugin
	const auto majorId = client->m_buffer->GetMajorId();
	auto map_msg = NetMsgDefine::GetNetMsg();
	auto plugin_name = map_msg[majorId];

	// get all the servers that the plugin exists
	auto config_file = m_app->GetConfigPtr();
	const auto server_list = config_file->GetServersByPluginName(plugin_name);

	// router
	const auto server_index = MsgRouter::GetMsgRouterByClient(plugin_name, static_cast<int>(server_list.size()), *this);

	// get server uuid
	auto server_map = ServerController::GetServerMap();
	auto server_uuid = server_map[server_list[server_index]->m_server_name];

	// send to backend server
	std::unique_ptr<IActorMsg> ptr = std::make_unique<ActorMsg<void, ClientNet, std::shared_ptr<ByteBuffer>>>(GetUUID(), server_uuid, &ClientNet::SendBuffer, std::move(m_buffer));
	m_app->SendMsgToActor(ptr);

	//
	m_buffer = std::make_shared<ByteBuffer>();
}

void NetProccessModule::ProcessBackendIO(IClientNetActor* client)
{
	// there is two situations, case 1: back msg to client; case 2: change client data
	if (m_buffer->GetMajorId() != static_cast<int>(NetMessage::ServerMsg::RETURN_CLIENT_MSG))
	{
		// unpack msg
		BackendMsgToClient backend_msg;
		ForEachField(backend_msg, m_buffer);

		// send to client
		// std::unique_ptr<IActorMsg> ptr = std::make_unique<ActorMsg<void, ClientNet, std::shared_ptr<ByteBuffer>>>(GetUUID(), backend_msg.m_client_uuid, &ClientNet::SendBuffer, std::move(backend_msg.m_buffer));
		// m_app->SendMsgToActor(ptr);
	}
	else if (m_buffer->GetMajorId() != static_cast<int>(NetMessage::ServerMsg::UPDATE_CLIENT_MSG))
	{

	}
	else
	{
		// error...
	}
}

void NetProccessModule::ProcessFrontendUnknowMsg(IClientNetActor* client)
{
	// check login info msg
	if (m_buffer->GetMajorId() != static_cast<int>(NetMessage::ServerMsg::LOGIN_MSG)) // LOGIN_MSG
	{
		return;
	}
}

void NetProccessModule::ProcessServerBackendIO(IClientNetActor* client)
{

}

void NetProccessModule::ProcessRPCIO(IClientNetActor* client)
{

}

void NetProccessModule::ProcessMasterIO(IClientNetActor* client)
{
	int majorId = m_buffer->GetMajorId();
	std::function<void(ClientDescriptor*)> callback = (*m_receive_callBack)[majorId];
	callback(this);
}