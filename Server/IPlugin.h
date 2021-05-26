#pragma once
#include <memory>
#include <map>
#include <string>
#include <iostream>
#include <cassert>

class IPluginManager;
class IModule;

class IPlugin
{
protected:
	// std::shared_ptr<IModuleManager> m_module_manager;
	std::map<std::string, std::shared_ptr<IModule>> m_map_module;
	std::shared_ptr<IPluginManager> m_ptr_manager;

	/// <summary>
	/// RegisterModule
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	void RegisterModule(std::shared_ptr<IPluginManager> ptr)
	{
		// auto pair = std::make_pair(std::string(typeid(T).name()), std::make_shared<T>(m_module_manager));
		// m_module_manager->AddModule(pair);

		auto pair = std::make_pair(std::string(typeid(T).name()), std::make_shared<T>(ptr));
		if (m_map_module.find(std::string(typeid(T).name())) != std::cend(m_map_module))
		{
			m_map_module.emplace(pair);
		}
		else
		{
			std::cout << std::string(typeid(T).name()) << std::endl;
			// assertm(0, "There are five lights");
			assert(0);
		}
	}
public:
	/// <summary>
	/// IPlugin
	/// </summary>
	/// <param name="app"></param>
	IPlugin(std::shared_ptr<IPluginManager> ptr)
	{
		m_ptr_manager = ptr;
	}

	//
	virtual void Install() = 0;
	virtual void Uninstall() = 0;

	//
	virtual void Init() = 0;
	virtual void AfterInit() = 0;
	virtual void ReadyExecute() = 0;
	virtual void Execute() = 0;
};
