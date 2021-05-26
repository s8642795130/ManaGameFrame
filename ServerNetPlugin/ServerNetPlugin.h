#pragma once
#include "../Server/Plugin.h"

class ServerNetPlugin : public Plugin
{
public:
	ServerNetPlugin(std::shared_ptr<IPluginManager> ptr) : Plugin(ptr)
	{

	}

	void Install() override;
	void Uninstall() override;
};

