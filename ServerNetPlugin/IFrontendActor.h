#pragma once
#include <queue>

#include "../Server/PackageNetMsg.h"
#include "../Server/BuiltInMsgDefine.h"
#include "NetActor.h"
#include "IBackendActor.h"

class IFrontendActor : public NetActor
{
protected:
	// client data
	std::string m_uid;
	std::map<std::string, std::string> m_client_data;

	// client msg list
	bool m_is_login = false;
	bool m_is_process_work = false;
	std::queue<FrontendMsg> m_queue_msg;
	
public:
	IFrontendActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr_impl, ITcpServer* ptr_sender) :
		NetActor(ptr_manager, ptr_impl, ptr_sender)
	{
	}

	bool CheckClientIsLogin()
	{
		bool ret = false;
		if (m_is_login)
		{
			ret = true;
		}
		else if (m_buffer->GetMajorId() == static_cast<int>(BuiltInMsg::ServerMsg::LOGIN_MSG)) // LOGIN_MSG
		{
			// check
			SetUid("");
			m_client_impl->m_client_net_module->AddLoginClientToMap(m_uid, GetUUID());
			ret = true;
		}

		return ret;
	}

	virtual void ProcessIO()
	{
		if (CheckClientIsLogin())
		{
			const auto major_id = m_buffer->GetMajorId();
			const auto& map_msg = m_client_impl->m_callback_module->GetGameMsgMap();
			auto plugin_name = map_msg.at(major_id);

			// get all the servers that the plugin exists
			const auto server_list = m_client_impl->m_config_module->GetServersByPluginName(plugin_name);

			// router
			const auto server_index = m_client_impl->m_router_module->GetMsgRouterByClient(plugin_name, static_cast<int>(server_list.size()), *this);

			// get server uuid
			auto server_uuid = m_client_impl->m_server_obj_module->GetServerUUIDByName(server_list[server_index]->m_server_name);

			// create backend client struct
			std::unique_ptr<FrontendToBackendMsg> backend_msg{ std::make_unique<FrontendToBackendMsg>() };
			backend_msg->m_client_uid = m_uid;
			backend_msg->m_client_data = m_client_data;
			backend_msg->m_major_id = m_buffer->GetMajorId();
			backend_msg->m_minor_id = m_buffer->GetMinorId();
			backend_msg->m_stream = m_buffer->GetStream();

			//
			FrontendMsg frontend_msg;
			frontend_msg.m_msg = std::move(backend_msg);
			frontend_msg.m_uuid = server_uuid;

			// send to backend server
			std::unique_ptr<IActorMsg> ptr = CreateActorMsg(GetUUID(), server_uuid, &IFrontendActor::ProcessNextIO, std::move(frontend_msg));
			GetActorPimpl()->SendMsgToActor(ptr);
		}
		else
		{
			// you need login
			ClientClose();
		}
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
			std::unique_ptr<IActorMsg> ptr = CreateActorMsg(GetUUID(), frontend_msg.m_uuid, &IBackendActor::SendStream, std::move(package));
			m_pimpl->SendMsgToActor(ptr);

			// del msg
			m_queue_msg.pop();
		}
	}
public:
	void SetUid(const std::string& uid)
	{
		m_uid = uid;
	}

	virtual const std::string& GetUid() const
	{
		return m_uid;
	}

	// client data
	const std::map<std::string, std::string> GetClientData() const
	{
		return m_client_data;
	}

	void UpdateClientData(const std::string key, const std::string value, int type)
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

	virtual void BackStream(const std::vector<char> stream) = 0;
};