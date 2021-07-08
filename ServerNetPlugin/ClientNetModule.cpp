#include "ClientNetModule.h"
#include "FrontendHttpActor.h"
#include "FrontendSocketActor.h"
#include "HPSocket.h"

void ClientNetModule::Init()
{
	m_client_pimpl = std::make_shared<ClientPimpl>();
	m_client_pimpl->m_config_module = m_ptr_manager->GetModule<IConfigModule>();
	m_client_pimpl->m_callback_module = m_ptr_manager->GetModule<INetCallbackModule>();
	m_client_pimpl->m_proccess_module = m_ptr_manager->GetModule<INetProccessModule>();
	m_client_pimpl->m_router_module = m_ptr_manager->GetModule<IMsgRouterModule>();
	m_client_pimpl->m_server_obj_module = m_ptr_manager->GetModule<IServerObjModule>();
	//
	m_config_module = m_ptr_manager->GetModule<IConfigModule>();
}

void ClientNetModule::AfterInit()
{
	if (m_config_module->GetServerType() == EnumDefine::ServerType::LOGIN ||
		(m_config_module->GetServerType() == EnumDefine::ServerType::FRONTEND && m_config_module->GetProtocolType() == EnumDefine::ProtocolType::WEBSOCKET))
	{
	}
}

// client interface

std::shared_ptr<INetActor> ClientNetModule::CreateHttpClientNet(ITcpServer* ptr_sender)
{
	std::shared_ptr<INetActor> client = std::make_shared<FrontendHttpActor>(m_ptr_manager, m_client_pimpl, ptr_sender);
	return client;
}

std::shared_ptr<INetActor> ClientNetModule::CreateSocketClientNet(ITcpServer* ptr_sender)
{
	std::shared_ptr<INetActor> client = std::make_shared<FrontendSocketActor>(m_ptr_manager, m_client_pimpl, ptr_sender);
	return client;
}

void ClientNetModule::AddClientToMap(std::shared_ptr<INetActor>& ptr)
{
	m_map_clients.emplace(ptr->GetSid(), ptr);
}

std::shared_ptr<INetActor> ClientNetModule::GetClientNet(const int& fd)
{
	return m_map_clients[fd];
}

void ClientNetModule::RemoveClientFromMap(const int& fd)
{
	std::map<int, std::shared_ptr<INetActor>>::const_iterator it = m_map_clients.find(fd);
	if (it != std::cend(m_map_clients))
	{
		m_map_clients.erase(it);
	}
}

// login client interface

void ClientNetModule::AddLoginClientToMap(std::string& uid, std::string& uuid)
{
	m_map_login_clients.Emplace(uid, uuid);
}

const std::string ClientNetModule::GetLoginClient(const std::string& uid)
{
	std::string uuid("");
	if (m_map_login_clients.find(uid) != std::cend(m_logged_in_clients))
	{
		ptr = m_logged_in_clients[uid];
	}
	return ptr;
}

void ClientNetModule::RemoveLoginClientFromMap(const std::string& uid)
{
	if (m_logged_in_clients.find(uid) != std::cend(m_logged_in_clients))
	{
		m_logged_in_clients.erase(uid);
	}
}