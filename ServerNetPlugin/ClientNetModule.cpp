#include "ClientNetModule.h"
#include "FrontendHttpActor.h"
#include "FrontendSocketActor.h"
#include "HPSocket.h"

void ClientNetModule::Init()
{
	// pImpl
	m_client_pimpl = std::make_shared<ClientPimpl>();
	m_client_pimpl->m_config_module = m_ptr_manager->GetModule<IConfigModule>();
	m_client_pimpl->m_callback_module = m_ptr_manager->GetModule<INetCallbackModule>();
	m_client_pimpl->m_proccess_module = m_ptr_manager->GetModule<INetProccessModule>();
	m_client_pimpl->m_router_module = m_ptr_manager->GetModule<IMsgRouterModule>();
	m_client_pimpl->m_server_obj_module = m_ptr_manager->GetModule<IServerObjModule>();
	m_client_pimpl->m_client_net_module = m_ptr_manager->GetModule<IClientNetModule>();
	// module
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

// login client interface

void ClientNetModule::AddLoginClientToMap(const std::string& uid, const std::string& uuid)
{
	m_map_login_clients.Emplace(uid, uuid);
}

const std::string ClientNetModule::GetLoginClient(const std::string& uid)
{
	std::string uuid("");
	uuid = m_map_login_clients.GetElement(uid);
	return uuid;
}

void ClientNetModule::RemoveLoginClientFromMap(const std::string& uid)
{
	m_map_login_clients.Erase(uid);
}