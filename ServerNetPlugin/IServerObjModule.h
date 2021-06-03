#pragma once
#include <map>
#include <string>

#include "../Server/IModule.h"

class IServerObjModule : public IModule
{
public:
	IServerObjModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	// life cycle
	virtual void Init() = 0;
	virtual void AfterInit() = 0;

	// interface
	virtual const std::string GetServerUUIDByName(const std::string& server_name) = 0;
};