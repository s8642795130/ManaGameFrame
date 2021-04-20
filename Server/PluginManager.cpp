#include <functional>
#include <iostream>

#include "PluginManager.h"

void PluginManager::LoadAllPluginLibrary()
{
	// for (const auto& item : m_map_lib)
	// {
		// std::function<void(void)> ptr_func{ reinterpret_cast<void*>(item.second->GetSymbol("DllStartPlugin")) };
		// ptr_func(this);
	// }

	// test code
	using MyTestFunc = std::shared_ptr<IPlugin>(*)();
	
	std::shared_ptr<DynLib> ptr_dll{ std::make_shared<DynLib>("libMasterPlugin") };
	ptr_dll->LoadLib();
	
	MyTestFunc func = (MyTestFunc)ptr_dll->GetSymbol("DllStartPlugin");
	if (func != nullptr)
	{
		m_map_lib.emplace("MasterPlugin", ptr_dll);
	}
}

bool PluginManager::LoadPluginLibrary(const std::string& pluginDLLName)
{
	// void(* DLL_START_PLUGIN_FUNC)(NFIPluginManager* pm)
	// for map

	/*
	for (const auto& item : m_map_lib)
	{
		// std::function<void(void)> ptr_func{ reinterpret_cast<void*>(item.second->GetSymbol("DllStartPlugin")) };
		// ptr_func(this);
	}
	*/

	// Load();
	// DLL_START_PLUGIN_FUNC pFunc = (DLL_START_PLUGIN_FUNC)pLib->GetSymbol("DllStartPlugin");
	// pFunc(this);
	return true;
}