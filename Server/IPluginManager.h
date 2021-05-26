#pragma once
#include <string>
#include <map>
#include <memory>

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
};

