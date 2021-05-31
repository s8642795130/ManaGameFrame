#pragma once
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <cassert>

class DynLib;
class IPlugin;
class IModule;

class IPluginManager : public std::enable_shared_from_this<IPluginManager>
{
protected:
	std::map<std::string, std::shared_ptr<DynLib>> m_map_lib;
	std::map<std::string, std::shared_ptr<IPlugin>> m_map_plugin;
	std::map<std::string, std::shared_ptr<IModule>> m_map_module;
public:
	template <typename T>
	std::shared_ptr<T> GetModule()
	{
		auto ptr = m_map_module[std::string(typeid(T).name())];
		return std::dynamic_pointer_cast<T>(ptr);
	}

	template <typename BaseT, typename T>
	void RegisterModule()
	{
		// auto pair = std::make_pair(std::string(typeid(T).name()), std::make_shared<T>(m_module_manager));
		// m_module_manager->AddModule(pair);

		auto pair = std::make_pair(std::string(typeid(BaseT).name()), std::make_shared<T>(shared_from_this()));
		if (m_map_module.find(std::string(typeid(BaseT).name())) == std::cend(m_map_module))
		{
			m_map_module.emplace(pair);
		}
		else
		{
			std::cout << std::string(typeid(BaseT).name()) << std::endl;
			// assertm(0, "There are five lights");
			assert(0);
		}
	}

	virtual const std::string& GetServerName() = 0;
};

