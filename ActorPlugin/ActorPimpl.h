#pragma once
#include <vector>

#include "IActorPimpl.h"
#include "IThreadPoolModule.h"
#include "../ServerNetPlugin/IMsgRouterModule.h"
#include "../ConfigPlugin/IConfigModule.h"
#include "../ServerNetPlugin/IServerObjModule.h"
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
public:
	ActorPimpl(std::shared_ptr<IPluginManager> ptr)
	{
		m_thread_pool_module = ptr->GetModule<IThreadPoolModule>();
		m_router_module = ptr->GetModule<IMsgRouterModule>();
		m_config_module = ptr->GetModule<IConfigModule>();
		m_server_obj_module = ptr->GetModule<IServerObjModule>();
	}

	virtual void SendMsgToActor(std::unique_ptr<IActorMsg>& actor_msg)
	{
		m_thread_pool_module->AddActorMsgToThreadCell(actor_msg);
	}

	virtual void RPCMsg()
	{

	}

	virtual void ResponseMsg(const std::string& uuid, const std::vector<char>& stream)
	{
		PushMsg(uuid, stream, EnumDefine::PushMsgType::RESPONSE);
	}

	virtual void NotifyMsg(const std::string& uuid, const std::vector<char>& stream)
	{
		PushMsg(uuid, stream, EnumDefine::PushMsgType::PUSH);
	}

	virtual void PushMsg(const std::string& uuid, const std::vector<char>& stream, const EnumDefine::PushMsgType type)
	{
		// build struct
		BackendMsgToClient backend_msg;
		backend_msg.m_msg_type = static_cast<int>(type);
		backend_msg.m_client_uuid = uuid;
		backend_msg.m_buffer = stream;

		// package
		std::vector<char> package;
		PackageStructForEachField(backend_msg, package);

		// router
		auto connector_server = m_config_module->GetServersByType(CONNECTOR);
		auto index = m_router_module->GetConnectorIndexByClient(connector_server.size(), uuid);

		// get frontend uuid
		auto frontend_uuid = m_server_obj_module->GetServerUUIDByName(connector_server[index]->m_server_name);

		// send to frontend
		std::unique_ptr<IActorMsg> actor_msg = std::make_unique<ActorMsg<void, IClientNetActor, std::vector<char>>>("", frontend_uuid, &IClientNetActor::SendData, static_cast<int>(BuiltInMsg::ServerMsg::RETURN_CLIENT_MSG), 0, std::move(package));
		m_thread_pool_module->AddActorMsgToThreadCell(actor_msg);
	}

	virtual void BroadcastMsg()
	{

	}

	virtual void UpdateClientData(const std::string& uuid, const std::string& key, const std::string& value, const EnumDefine::UpdateClientDataType type)
	{
		UpdateClient update_client;
		update_client.m_client_uuid = uuid;
		update_client.m_data_key = key;
		update_client.m_data_value = value;
		update_client.m_update_type = static_cast<int>(type);

		// package
		std::vector<char> package;
		PackageStructForEachField(update_client, package);

		// router
		auto connector_server = m_config_module->GetServersByType(CONNECTOR);
		auto index = m_router_module->GetConnectorIndexByClient(connector_server.size(), uuid);

		// get frontend uuid
		auto frontend_uuid = m_server_obj_module->GetServerUUIDByName(connector_server[index]->m_server_name);

		// send to frontend
		std::unique_ptr<IActorMsg> actor_msg = std::make_unique<ActorMsg<void, IClientNetActor, const int, const int, std::vector<char>>>("", frontend_uuid, &IClientNetActor::SendData, static_cast<int>(BuiltInMsg::ServerMsg::UPDATE_CLIENT_DATA), 0, std::move(package));
		m_thread_pool_module->AddActorMsgToThreadCell(actor_msg);
	}
};