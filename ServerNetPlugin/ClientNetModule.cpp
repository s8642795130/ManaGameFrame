#include "ClientNetModule.h"
#include "ClientNetActor.h"
#include "ClientHttpActor.h"

void ClientNetModule::Init()
{
	m_client_pimpl = std::make_shared<ClientPimpl>();
	m_client_pimpl->m_config_module = m_ptr_manager->GetModule<IConfigModule>();
	m_client_pimpl->m_proccess_module = m_ptr_manager->GetModule<INetProccessModule>();
	//
	m_config_module = m_ptr_manager->GetModule<IConfigModule>();
}

void ClientNetModule::AfterInit()
{
	if (m_config_module->GetServerType() == EnumDefine::ServerType::LOGIN)
	{
		m_create_net = [this]() -> std::shared_ptr<IClientNetActor>
		{
			return CreateHttpClientNet();
		};
	}
	else
	{
		m_create_net = [this]() -> std::shared_ptr<IClientNetActor>
		{
			return CreateSocketClientNet();
		};
	}
}

// client interface

std::shared_ptr<IClientNetActor> ClientNetModule::CreateClientNet()
{
	return m_create_net();
}

std::shared_ptr<IClientNetActor> ClientNetModule::CreateHttpClientNet()
{
	std::shared_ptr<IClientNetActor> client = std::make_shared<ClientHttpActor>(m_ptr_manager, m_client_pimpl);
	return client;
}

std::shared_ptr<IClientNetActor> ClientNetModule::CreateSocketClientNet()
{
	std::shared_ptr<IClientNetActor> client = std::make_shared<ClientNetActor>(m_ptr_manager, m_client_pimpl);
	return client;
}

void ClientNetModule::AddClientToMap(std::shared_ptr<IClientNetActor>& ptr)
{
	m_map_clients.emplace(ptr->GetSid(), ptr);
}

std::shared_ptr<IClientNetActor> ClientNetModule::GetClientNet(const int& fd)
{
	return m_map_clients[fd];
}

void ClientNetModule::RemoveClientFromMap(const int& fd)
{
	std::map<int, std::shared_ptr<IClientNetActor>>::const_iterator it = m_map_clients.find(fd);
	if (it != std::cend(m_map_clients))
	{
		m_map_clients.erase(it);
	}
}

// login client interface

void ClientNetModule::AddLoginClientToMap(std::shared_ptr<IClientNetActor>& ptr)
{
	m_logged_in_clients.emplace(ptr->GetUid(), ptr);
}

std::shared_ptr<IClientNetActor> ClientNetModule::GetLoginClient(const std::string& uid)
{
	std::shared_ptr<IClientNetActor> ptr = nullptr;
	if (m_logged_in_clients.find(uid) != std::cend(m_logged_in_clients))
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