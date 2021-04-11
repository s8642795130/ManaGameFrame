#pragma once
#include "../Server/Plugin.h"

class MasterPlugin : public Plugin
{
public:
	MasterPlugin(std::shared_ptr<IApplication> app) : Plugin(app)
	{

	}

	void Install() override;
	void Uninstall() override;
};

