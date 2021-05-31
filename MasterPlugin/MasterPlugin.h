#pragma once
#include "../Server/IPlugin.h"

class MasterPlugin : public IPlugin
{
public:
	MasterPlugin(std::shared_ptr<IPluginManager> app) : IPlugin(app)
	{

	}

	void Install() override;
	void Uninstall() override;
};

