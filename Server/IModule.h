#pragma once
#include <memory>

// #include "IModuleManager.h"
#include "IPlugin.h"

class IModule
{
protected:
	std::shared_ptr<IPlugin> m_plugin;
public:
	explicit IModule(std::shared_ptr<IPlugin> ptr) : m_plugin(ptr)
	{
	}

	virtual ~IModule() = default;

	virtual void Init() = 0;
	virtual void AfterInit() = 0;
	virtual void ReadyExecute() = 0;
	virtual void Execute() = 0;
};

