#pragma once
#include <queue>

#include "INetActor.h"
#include "ClientPimpl.h"

class IFrontendActor : public INetActor
{
protected:
	// impl
	std::shared_ptr<ClientPimpl> m_client_impl;

	// client data
	std::string m_uid;
	std::map<std::string, std::string> m_client_data;

	// client msg list
	bool m_is_process_work = false;
	std::queue<FrontendMsg> m_queue_msg;
	
protected:
	virtual void ProccessIO()
	{
		const auto major_id = client.GetBuffer()->GetMajorId();
		auto map_msg = m_callback_module->GetGameMsgMap();
		auto plugin_name = map_msg[major_id];

		// get all the servers that the plugin exists
		const auto server_list = m_config_module->GetServersByPluginName(plugin_name);

		// router
		const auto server_index = m_router_module->GetMsgRouterByClient(plugin_name, static_cast<int>(server_list.size()), client);

		// get server uuid
		auto server_uuid = m_server_obj_module->GetServerUUIDByName(server_list[server_index]->m_server_name);

		// create backend client struct
		std::unique_ptr<FrontendToBackendMsg> backend_msg{ std::make_unique<FrontendToBackendMsg>() };
		auto buffer = client.GetBuffer();
		backend_msg->m_client_uid = client.GetUid();
		backend_msg->m_client_data = client.GetClientData();
		backend_msg->m_major_id = buffer->GetMajorId();
		backend_msg->m_minor_id = buffer->GetMinorId();
		backend_msg->m_stream = buffer->GetStream();

		//
		FrontendMsg frontend_msg;
		frontend_msg.m_msg = std::move(backend_msg);
		frontend_msg.m_uuid = server_uuid;

		// send to backend server
		std::unique_ptr<IActorMsg> ptr = CreateActorMsg(client.GetUUID(), server_uuid, &IClientNetActor::ProcessNextIO, std::move(frontend_msg));
		client.GetActorPimpl()->SendMsgToActor(ptr);
	}

	void ProcessNextIO(FrontendMsg frontend_msg)
	{
		m_queue_msg.push(frontend_msg);
		if (m_is_process_work == false)
		{
			m_is_process_work = true;
			NextIO();
		}
	}

	void NextIO()
	{
		// get queue front msg
		if (m_queue_msg.empty())
		{
			m_is_process_work = false;
		}
		else
		{
			const FrontendMsg& frontend_msg = m_queue_msg.front();

			// package msg
			std::vector<char> package;
			PackageStructForEachField(*frontend_msg.m_msg, package); // dereference

			// send to backend server
			std::unique_ptr<IActorMsg> ptr = CreateActorMsg(GetUUID(), frontend_msg.m_uuid, &IClientNetActor::SendStream, std::move(package));
			m_pimpl->SendMsgToActor(ptr);

			// del msg
			m_queue_msg.pop();
		}
	}
public:
	// client data
	virtual const std::map<std::string, std::string> GetClientData() const;

	void UpdateClientData(const std::string& key, const std::string& value, int type)
	{
		if (type == static_cast<int>(EnumDefine::UpdateClientDataType::MODIFY))
		{
			m_client_data[key] = value;
		}
		else
		{
			m_client_data.erase(key);
		}
	}
};