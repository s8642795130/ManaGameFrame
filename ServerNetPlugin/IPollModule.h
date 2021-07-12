#pragma once
#include "../Server/IModule.h"

class IPollClient;

class IPollModule : public IModule
{
public:
	IPollModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	// interface
	virtual void EventLoop() = 0;
	virtual bool ConnectServerWithServerName(const std::string& ip, const int port, const std::string& server_name) = 0;
	virtual std::shared_ptr<IPollClient> GetClientByServerName(const std::string& server_name) = 0;
};