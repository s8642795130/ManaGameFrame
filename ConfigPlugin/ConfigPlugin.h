#pragma once
#include "../Server/Plugin.h"

class ConfigPlugin : public Plugin
{
public:
	ConfigPlugin(std::shared_ptr<IPluginManager> ptr) : Plugin(ptr)
	{

	}

	void Install() override;
	void Uninstall() override;
};