#pragma once
#include "../Server/IPlugin.h"

class ActorPlugin : public IPlugin
{
public:
	ActorPlugin(std::shared_ptr<IPluginManager> ptr) : IPlugin(ptr)
	{

	}

	void Install() override;
	void Uninstall() override;
};