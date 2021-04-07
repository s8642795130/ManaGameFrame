#include <algorithm>

#include "Plugin.h"
#include "IModule.h"

void Plugin::Init()
{
	std::for_each(std::begin(m_map_module), std::end(m_map_module), [](auto& it) -> void
		{
			(it.second)->Init();
		}
	);
}

void Plugin::AfterInit()
{
	std::for_each(std::begin(m_map_module), std::end(m_map_module), [](auto& it) -> void
		{
			(it.second)->AfterInit();
		}
	);
}

void Plugin::ReadyExecute()
{
	std::for_each(std::begin(m_map_module), std::end(m_map_module), [](auto& it) -> void
		{
			(it.second)->ReadyExecute();
		}
	);
}

void Plugin::Execute()
{
	std::for_each(std::begin(m_map_module), std::end(m_map_module), [](auto& it) -> void
		{
			(it.second)->Execute();
		}
	);
}