#include <functional>
#include <iostream>

#include "PluginManager.h"

// test code
void PluginManager::LoadAllPluginLibrary(int test_code)
{
	// for (const auto& item : m_map_lib)
	// {
		// std::function<void(void)> ptr_func{ reinterpret_cast<void*>(item.second->GetSymbol("DllStartPlugin")) };
		// ptr_func(this);
	// }

	// test code
	using MyTestFunc = std::shared_ptr<IPlugin>(*)(std::shared_ptr<IApplication>);

	std::string lib_name;
	if (test_code == 1)
	{
		lib_name = "MasterPlugin";
	}
	else
	{
		lib_name = "ServerControlPlugin";
	}

	std::shared_ptr<DynLib> ptr_dll{ std::make_shared<DynLib>("lib" + lib_name) };
	ptr_dll->LoadLib();
	
	MyTestFunc func = (MyTestFunc)ptr_dll->GetSymbol("DllStartPlugin");
	if (func != nullptr)
	{
		m_map_lib.emplace(lib_name, ptr_dll);
		m_map_plugin.emplace(lib_name, func(IApplication::GetPtr()));
	}

	RegisterAll();
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

void PluginManager::RegisterAll()
{
	for (const auto& item : m_map_plugin)
	{
		item.second->Install();
	}
}

void PluginManager::Init()
{
	for (const auto& item : m_map_plugin)
	{
		item.second->Init();
	}
}

void PluginManager::AfterInit()
{
	for (const auto& item : m_map_plugin)
	{
		item.second->AfterInit();
	}
}

void PluginManager::ReadyExecute()
{
	for (const auto& item : m_map_plugin)
	{
		item.second->ReadyExecute();
	}
}

void PluginManager::Execute()
{
	for (const auto& item : m_map_plugin)
	{
		item.second->Execute();
	}
}