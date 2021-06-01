#include "ClientNetModule.h"
#include "ClientNetActor.h"

void ClientNetModule::Init()
{
	m_client_pimpl = std::make_shared<ClientPimpl>();
	m_client_pimpl->m_config_module = m_ptr_manager->GetModule<IConfigModule>();
	m_client_pimpl->m_proccess_module = m_ptr_manager->GetModule<INetProccessModule>();
}

std::shared_ptr<IClientNetActor> ClientNetModule::CreateClientNet()
{
	std::shared_ptr<IClientNetActor> client = std::make_shared<ClientNetActor>(m_client_pimpl);
	return client;
}

void ClientNetModule::AddClientToMap(std::shared_ptr<IClientNetActor>& ptr)
{
	m_map_clients.emplace(ptr->GetSid(), ptr);
}

void ClientNetModule::RemoveClientFromMap(const int& fd)
{
	std::map<int, std::shared_ptr<IClientNetActor>>::iterator it = m_map_clients.find(fd);
	if (it != std::cend(m_map_clients))
	{
		m_map_clients.erase(it);
	}
}

std::shared_ptr<IClientNetActor> ClientNetModule::GetClientNet(const int& fd)
{
	return m_map_clients[fd];
}