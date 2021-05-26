#pragma once
#include <memory>

#include "IPluginManager.h"

class IModule
{
protected:
	std::shared_ptr<IPluginManager> m_ptr_manager;
public:
	explicit IModule(std::shared_ptr<IPluginManager> ptr) : m_ptr_manager(ptr)
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

