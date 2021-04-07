#include <functional>

#include "PluginManager.h"

bool PluginManager::LoadPluginLibrary(const std::string& pluginDLLName)
{
	// void(* DLL_START_PLUGIN_FUNC)(NFIPluginManager* pm)
	// for map
	for (const auto& item : m_map_lib)
	{
		std::function<void(void)> ptr_func{ reinterpret_cast<void*>(item.second->GetSymbol("DllStartPlugin")) };
		// ptr_func(this);
	}
	// Load();
	// DLL_START_PLUGIN_FUNC pFunc = (DLL_START_PLUGIN_FUNC)pLib->GetSymbol("DllStartPlugin");
	// pFunc(this);
	return true;
}