#pragma once
#include "../Server/IPlugin.h"

class LoginPlugin : public IPlugin
{
public:
	LoginPlugin(std::shared_ptr<IPluginManager> app) : IPlugin(app)
	{

	}

	void Install() override;
	void Uninstall() override;
};

