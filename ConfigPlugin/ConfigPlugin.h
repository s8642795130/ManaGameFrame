#pragma once
#include "../Server/IPlugin.h"

class ConfigPlugin : public IPlugin
{
public:
	ConfigPlugin(std::shared_ptr<IPluginManager> ptr) : IPlugin(ptr)
	{

	}

	void Install() override;
	void Uninstall() override;
};