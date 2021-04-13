#pragma once
#include "../Server/Plugin.h"

class ServerControlPlugin : public Plugin
{
	ServerControlPlugin(std::shared_ptr<IApplication> app) : Plugin(app)
	{

	}

	void Install() override;
	void Uninstall() override;
};

