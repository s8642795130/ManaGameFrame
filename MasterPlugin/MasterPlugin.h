#pragma once
#include "../Server/Plugin.h"

class MasterPlugin : public Plugin
{
public:
	void Install() override;
	void Uninstall() override;
};

