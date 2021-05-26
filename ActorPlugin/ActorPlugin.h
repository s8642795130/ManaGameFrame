#pragma once
#include "../Server/Plugin.h"

class ActorPlugin : public Plugin
{
public:
	ActorPlugin(std::shared_ptr<IPluginManager> ptr) : Plugin(ptr)
	{

	}

	void Install() override;
	void Uninstall() override;
};