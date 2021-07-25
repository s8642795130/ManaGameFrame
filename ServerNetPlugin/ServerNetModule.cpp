#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <vector>
#include <errno.h>
#include <stdexcept>
#include <unistd.h>
#include <time.h>

#include <string>
#include <map>
#include <functional>
#include <iostream>

#include "ServerNetModule.h"
#include "HPSocket.h"
#include "FrontendListenerImpl.h"
#include "MasterListenerImpl.h"
#include "BackendListenerImpl.h"

ServerNetModule::ServerNetModule(std::shared_ptr<IPluginManager> ptr) :
	IServerNetModule(ptr)
{
}

ServerNetModule::~ServerNetModule()
{
	m_server->Get()->Stop();
}

void ServerNetModule::Init()
{
	// init
	m_config_module = m_ptr_manager->GetModule<IConfigModule>();
	m_thread_pool_module = m_ptr_manager->GetModule<IThreadPoolModule>();
	m_client_net_module = m_ptr_manager->GetModule<IClientNetModule>();
}

void ServerNetModule::AfterInit()
{
	// auto port = m_config_module->GetMyServerInfo()->m_port;
	// StartNetwork(static_cast<uint16_t>(port), 30);

	std::cout << "m_config_module->GetServerType() " << static_cast<int>(m_config_module->GetServerType()) << std::endl;

	// server
	if (m_config_module->GetServerType() == EnumDefine::ServerType::LOGIN ||
		(m_config_module->GetServerType() == EnumDefine::ServerType::FRONTEND && m_config_module->GetProtocolType() == EnumDefine::ProtocolType::WEBSOCKET))
	{
	}
	else if (m_config_module->GetServerType() == EnumDefine::ServerType::FRONTEND && m_config_module->GetProtocolType() == EnumDefine::ProtocolType::SOCKET)
	{
		m_ptr_listener = std::make_shared<CFrontendListenerImpl>();
	}
	else if (m_config_module->GetServerType() == EnumDefine::ServerType::MASTER)
	{
		m_ptr_listener = std::make_shared<CMasterListenerImpl>();
	}
	else if (m_config_module->GetServerType() == EnumDefine::ServerType::BACKEND)
	{
		m_ptr_listener = std::make_shared<CBackendListenerImpl>();
	}
	
	m_ptr_listener->SetManagerPtr(m_ptr_manager);
	m_ptr_listener->Init();

	// network
	m_server = std::make_shared<CTcpServerPtr>(m_ptr_listener.get());
	auto port = m_config_module->GetMyServerInfo()->m_port;
	m_server->Get()->Start("0.0.0.0", static_cast<USHORT>(port));
}