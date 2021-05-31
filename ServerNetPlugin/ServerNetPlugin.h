#pragma once
#include "../Server/IPlugin.h"

class ServerNetPlugin : public IPlugin
{
public:
	ServerNetPlugin(std::shared_ptr<IPluginManager> ptr) : IPlugin(ptr)
	{

	}

	void Install() override;
	void Uninstall() override;
};

