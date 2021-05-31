#include "ServerObjModule.h"

void ServerObjModule::Init()
{
	m_client_net_module = m_ptr_manager->GetModule<IClientNetModule>();
}

void ServerObjModule::AfterInit()
{

}

// interface
const std::map<std::string, std::string>& ServerObjModule::GetServerMap()
{
	return m_map_server;
}