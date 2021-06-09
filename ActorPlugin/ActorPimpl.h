#pragma once
#include <vector>

#include "IActorPimpl.h"
#include "IThreadPoolModule.h"
#include "../ServerNetPlugin/IMsgRouterModule.h"
#include "../ConfigPlugin/IConfigModule.h"
#include "../ServerNetPlugin/IServerObjModule.h"
#include "../ServerNetPlugin/INetCallbackModule.h"
#include "../ActorPlugin/ActorMsg.h"
#include "../Server/BuiltInDataDefine.h"
#include "../Server/BuiltInMsgDefine.h"
#include "../Server/PackageNetMsg.h"
#include "../Server/StringDefine.h"
#include "../Server/ServerEnumDefine.h"

class ActorPimpl : public IActorPimpl
{
protected:
	std::shared_ptr<IThreadPoolModule> m_thread_pool_module;
	std::shared_ptr<IMsgRouterModule> m_router_module;
	std::shared_ptr<IConfigModule> m_config_module;
	std::shared_ptr<IServerObjModule> m_server_obj_module;
	std::shared_ptr<INetCallbackModule> m_callback_module;
	// rpc callback
	std::string m_uuid;
	int m_callback_id = 0;
	std::map<int, std::function<void()>> m_rpc_callback;
protected:
	void PushMsg(const std::string& uid, const std::vector<char>& stream, const EnumDefine::PushMsgType type)
	{
		// build struct
		BackendMsgToClient backend_msg;
		backend_msg.m_msg_type = static_cast<int>(type);
		backend_msg.m_client_uid = uid;
		backend_msg.m_buffer = stream;

		// package
		std::vector<char> package;
		PackageStructForEachField(backend_msg, package);

		// router
		auto connector_server = m_config_module->GetServersByType(CONNECTOR);
		auto index = m_router_module->GetConnectorIndexByClient(static_cast<int>(connector_server.size()), uid);

		// get frontend uuid
		auto frontend_uuid = m_server_obj_module->GetServerUUIDByName(connector_server[index]->m_server_name);

		// send to frontend
		std::unique_ptr<IActorMsg> actor_msg = std::make_unique<ActorMsg<void, IClientNetActor, const int, const int, std::vector<char>>>("", frontend_uuid, &IClientNetActor::SendData, static_cast<int>(BuiltInMsg::ServerMsg::RETURN_CLIENT_MSG), 0, std::move(package));
		m_thread_pool_module->AddActorMsgToThreadCell(actor_msg);
	}
public:
	ActorPimpl(std::shared_ptr<IPluginManager>& ptr)
	{
		m_thread_pool_module = ptr->GetModule<IThreadPoolModule>();
		m_router_module = ptr->GetModule<IMsgRouterModule>();
		m_config_module = ptr->GetModule<IConfigModule>();
		m_server_obj_module = ptr->GetModule<IServerObjModule>();
		m_callback_module = ptr->GetModule<INetCallbackModule>();
	}

	virtual void SetUUID(const std::string& uuid)
	{
		m_uuid = uuid;
	}

	virtual void SendMsgToActor(std::unique_ptr<IActorMsg>& ptr_actor_msg)
	{
		m_thread_pool_module->AddActorMsgToThreadCell(ptr_actor_msg);
	}

	virtual void RPCMsg(const int major_id, const int minor_id, const std::vector<char>& stream, std::function<void(std::vector<char>)> callback, const std::string& router = "")
	{
		RPCMsgData rpc_data;
		rpc_data.m_major_id = major_id;
		rpc_data.m_minor_id = minor_id;
		rpc_data.m_stream = stream;
		rpc_data.m_uuid = m_uuid;
		rpc_data.m_callback_id = ++m_callback_id;

		// get msg corresponding to plugin
		auto map_msg = m_callback_module->GetGameMsgMap();
		auto plugin_name = map_msg[major_id];

		// get all the servers that the plugin exists
		const auto server_list = m_config_module->GetServersByPluginName(plugin_name);

		// router
		const auto server_index = m_router_module->GetMsgRouterByString(plugin_name, static_cast<int>(server_list.size()), router);

		// check ! server name is the same as the local server name

		// get server uuid
		auto server_uuid = m_server_obj_module->GetServerUUIDByName(server_list[server_index]->m_server_name);

		// package
		std::vector<char> package;
		PackageStructForEachField(rpc_data, package);

		// send to backend server
		std::unique_ptr<IActorMsg> ptr_actor_msg = std::make_unique<ActorMsg<void, IClientNetActor, const int, const int, std::vector<char>>>(m_uuid, server_uuid, &IClientNetActor::SendData, std::move(package));
		m_thread_pool_module->AddActorMsgToThreadCell(ptr_actor_msg);
	}

	virtual void ResponseRPC(const std::vector<char>& stream, const std::string& uuid, const int callback_id)
	{

	}

	virtual void ResponseMsg(const std::string& uid, const std::vector<char>& stream)
	{
		PushMsg(uid, stream, EnumDefine::PushMsgType::RESPONSE);
	}

	virtual void NotifyMsg(const std::string& uid, const std::vector<char>& stream)
	{
		PushMsg(uid, stream, EnumDefine::PushMsgType::PUSH);
	}

	virtual void BroadcastMsg(const std::vector<std::string>& vec_uid, const std::vector<char>& stream)
	{
		for (const auto& item : vec_uid)
		{
			PushMsg(item, stream, EnumDefine::PushMsgType::PUSH);
		}
	}

	virtual void UpdateClientData(const std::string& uid, const std::string& key, const std::string& value, const EnumDefine::UpdateClientDataType type)
	{
		UpdateClient update_client;
		update_client.m_client_uid = uid;
		update_client.m_data_key = key;
		update_client.m_data_value = value;
		update_client.m_update_type = static_cast<int>(type);

		// package
		std::vector<char> package;
		PackageStructForEachField(update_client, package);

		// router
		auto connector_server = m_config_module->GetServersByType(CONNECTOR);
		auto index = m_router_module->GetConnectorIndexByClient(static_cast<int>(connector_server.size()), uid);

		// get frontend uuid
		auto frontend_uuid = m_server_obj_module->GetServerUUIDByName(connector_server[index]->m_server_name);

		// send to frontend
		std::unique_ptr<IActorMsg> actor_msg = std::make_unique<ActorMsg<void, IClientNetActor, const int, const int, std::vector<char>>>("", frontend_uuid, &IClientNetActor::SendData, static_cast<int>(BuiltInMsg::ServerMsg::UPDATE_CLIENT_DATA), 0, std::move(package));
		m_thread_pool_module->AddActorMsgToThreadCell(actor_msg);
	}
};