#pragma once
#include <map>
#include <string>

#include "../Server/IModule.h"

class IServerObjModule : IModule
{
public:
	IServerObjModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	// interface
	virtual const std::map<std::string, std::string>& GetServerMap() = 0;
};