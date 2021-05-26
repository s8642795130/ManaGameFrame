#pragma once
#include "../Server/Plugin.h"

class MasterPlugin : public Plugin
{
public:
	MasterPlugin(std::shared_ptr<IPluginManager> app) : Plugin(app)
	{

	}

	void Install() override;
	void Uninstall() override;
};

