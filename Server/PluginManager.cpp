#include <functional>
#include <iostream>
#include <unistd.h>

#include "PluginManager.h"
#include "IModule.h"
#include "CommonStruct.h"
#include "DefineHeader.h"
#include "StringDefine.h"

void PluginManager::GetRealPath()
{
	std::array<char, MAX_REAL_PATH> abs_path_buff;
	auto cnt = readlink("/proc/self/exe", abs_path_buff.data(), MAX_REAL_PATH);
	if (cnt < 0 || cnt >= MAX_REAL_PATH)
	{
		std::perror(CANNOT_GET_REAL_PATH);
		std::terminate();
	}
	else
	{
		std::string temp_path{ abs_path_buff.data() };
		auto pos = temp_path.rfind('/');
		m_real_path = temp_path.substr(0, pos);
	}
}

void PluginManager::LoadAllPluginLibrary(std::vector<std::shared_ptr<PluginData>> vec_plugin)
{
	GetRealPath();

	for (const auto& item : vec_plugin)
	{
		LoadPluginLibrary(item->m_plugin);
	}

	RegisterAll();
}

bool PluginManager::LoadPluginLibrary(const std::string& plugin_dll_name)
{
	// 
	using MyFunc = std::shared_ptr<IPlugin>(*)(std::shared_ptr<IPluginManager>);

	//
	std::shared_ptr<DynLib> ptr_dll{ std::make_shared<DynLib>(m_real_path + "/lib" + plugin_dll_name + ".so") };
	ptr_dll->LoadLib();

	//
	MyFunc func = (MyFunc)ptr_dll->GetSymbol("DllStartPlugin");
	if (func != nullptr)
	{
		m_map_lib.emplace(plugin_dll_name, ptr_dll);
		m_map_plugin.emplace(plugin_dll_name, func(shared_from_this()));
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

// config

void PluginManager::SetServerName(const std::string& server_name)
{
	m_server_name = server_name;
}

const std::string& PluginManager::GetServerName()
{
	return m_server_name;
}

// runtime

void PluginManager::Init()
{
	std::for_each(std::begin(m_map_module), std::end(m_map_module), [](auto& it) -> void
		{
			(it.second)->Init();
		}
	);
}

void PluginManager::AfterInit()
{
	std::for_each(std::begin(m_map_module), std::end(m_map_module), [](auto& it) -> void
		{
			(it.second)->AfterInit();
		}
	);
}

void PluginManager::ReadyExecute()
{
	std::for_each(std::begin(m_map_module), std::end(m_map_module), [](auto& it) -> void
		{
			(it.second)->ReadyExecute();
		}
	);
}

void PluginManager::Execute()
{
	std::for_each(std::begin(m_map_module), std::end(m_map_module), [](auto& it) -> void
		{
			(it.second)->Execute();
		}
	);
}