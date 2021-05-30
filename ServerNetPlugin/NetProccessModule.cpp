#include "NetProccessModule.h"
#include "IClientNetActor.h"
#include "../Server/BuiltInMsgDefine.h"
#include "../Server/BuiltInDataDefine.h"
#include "../Server/UnpackNetMsg.h"
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
	auto server_map = m_server_obj_module->GetServerMap();
	auto server_uuid = server_map[server_list[server_index]->m_server_name];

	// send to backend server
	std::unique_ptr<IActorMsg> ptr = std::make_unique<ActorMsg<void, IClientNetActor, std::shared_ptr<ByteBuffer>>>(client.GetUUID(), server_uuid, &IClientNetActor::SendBuffer, std::move(client.GetBuffer()));
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
	else if (client.GetBuffer()->GetMajorId() != static_cast<int>(BuiltInMsg::ServerMsg::UPDATE_CLIENT_MSG))
	{

	}
	else
	{
		// error...
	}
}

void NetProccessModule::ProcessFrontendUnknowMsg(IClientNetActor& client)
{
	// check login info msg
	if (client.GetBuffer()->GetMajorId() != static_cast<int>(BuiltInMsg::ServerMsg::LOGIN_MSG)) // LOGIN_MSG
	{
		// check

		//
		return;
	}
}

void NetProccessModule::ProcessServerBackendIO(IClientNetActor& client)
{

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