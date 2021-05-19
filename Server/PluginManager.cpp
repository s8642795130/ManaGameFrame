#include <functional>
#include <iostream>

#include "PluginManager.h"
#include "CommonStruct.h"

void PluginManager::LoadAllPluginLibrary(std::vector<std::shared_ptr<PluginData>> vec_plugin)
{
	for (const auto& item : vec_plugin)
	{
		LoadPluginLibrary(item->m_plugin);
	}

	RegisterAll();
}

bool PluginManager::LoadPluginLibrary(const std::string& plugin_dll_name)
{
	using MyFunc = std::shared_ptr<IPlugin>(*)(std::shared_ptr<IApplication>);

	std::shared_ptr<DynLib> ptr_dll{ std::make_shared<DynLib>("lib" + plugin_dll_name) };
	ptr_dll->LoadLib();

	MyFunc func = (MyFunc)ptr_dll->GetSymbol("DllStartPlugin");
	if (func != nullptr)
	{
		m_map_lib.emplace(plugin_dll_name, ptr_dll);
		m_map_plugin.emplace(plugin_dll_name, func(IApplication::GetPtr()));
	}

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