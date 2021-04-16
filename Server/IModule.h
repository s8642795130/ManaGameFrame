#pragma once
#include <memory>

// #include "IModuleManager.h"
#include "IPlugin.h"
#include "IApplication.h"

class IModule
{
protected:
	std::shared_ptr<IPlugin> m_plugin;
public:
	explicit IModule(std::shared_ptr<IPlugin> ptr) : m_plugin(ptr)
	{
	}

	virtual ~IModule() = default;

	virtual void Init()
	{
	}

	virtual void AfterInit()
	{
	}

	virtual void ReadyExecute()
	{
	}

	virtual void Execute()
	{

	}
};

