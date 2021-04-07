#pragma once
#include "IPlugin.h"
class Plugin : public IPlugin
{
public:
	virtual void Init();
	virtual void AfterInit();
	virtual void ReadyExecute();
	virtual void Execute();
};

