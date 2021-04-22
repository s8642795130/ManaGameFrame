#pragma once
#include <memory>
#include <map>
#include <string>
#include <iostream>

#include "IApplication.h"

class IModule;

class IPlugin : public std::enable_shared_from_this<IPlugin>
{
protected:
	// std::shared_ptr<IModuleManager> m_module_manager;
	std::map<std::string, std::shared_ptr<IModule>> m_map_module;
	std::shared_ptr<IApplication> m_app;

	/// <summary>
	/// RegisterModule
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	void RegisterModule(std::shared_ptr<IPlugin> ptr)
	{
		// auto pair = std::make_pair(std::string(typeid(T).name()), std::make_shared<T>(m_module_manager));
		// m_module_manager->AddModule(pair);

		auto pair = std::make_pair(std::string(typeid(T).name()), std::make_shared<T>(ptr));
		m_map_module.emplace(pair);
	}
public:
	/// <summary>
	/// IPlugin
	/// </summary>
	/// <param name="app"></param>
	IPlugin(std::shared_ptr<IApplication> app)
	{
		m_app = app;
	}

	virtual void Install() = 0;
	virtual void Uninstall() = 0;

	std::shared_ptr<IApplication>& GetAppPtr()
	{
		return m_app;
	}

	/// <summary>
	/// GetModule
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template <typename T>
	std::shared_ptr<T> GetModule()
	{
		auto ptr = m_map_module[std::string(typeid(T).name())];
		return std::dynamic_pointer_cast<T>(ptr);
	}

	virtual void Init() = 0;
	virtual void AfterInit() = 0;
	virtual void ReadyExecute() = 0;
	virtual void Execute() = 0;
};
