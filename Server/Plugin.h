#pragma once
#include <algorithm>

#include "IPlugin.h"
#include "IModule.h"

class IPluginManager;

class Plugin : public IPlugin
{
public:
	Plugin(std::shared_ptr<IPluginManager> ptr) : IPlugin(ptr)
	{

	}


	virtual void Init()
	{
		std::for_each(std::begin(m_map_module), std::end(m_map_module), [](auto& it) -> void
			{
				(it.second)->Init();
			}
		);
	}

	virtual void AfterInit()
	{
		std::for_each(std::begin(m_map_module), std::end(m_map_module), [](auto& it) -> void
			{
				(it.second)->AfterInit();
			}
		);
	}

	virtual void ReadyExecute()
	{
		std::for_each(std::begin(m_map_module), std::end(m_map_module), [](auto& it) -> void
			{
				(it.second)->ReadyExecute();
			}
		);
	}

	virtual void Execute()
	{
		std::for_each(std::begin(m_map_module), std::end(m_map_module), [](auto& it) -> void
			{
				(it.second)->Execute();
			}
		);
	}
};

