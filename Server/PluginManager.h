#pragma once
#include <string>
#include <map>
#include <memory>

#include "IPlugin.h"
#include "DynLib.h"

struct PluginData;

class PluginManager
{
private:
	std::map<std::string, std::shared_ptr<DynLib>> m_map_lib;
	std::map<std::string, std::shared_ptr<IPlugin>> m_map_plugin;
public:
	void LoadAllPluginLibrary(std::vector<std::shared_ptr<PluginData>> vec_plugin);
	bool LoadPluginLibrary(const std::string& pluginDLLName);
	void RegisterAll();

	void Init();
	void AfterInit();
	void ReadyExecute();
	void Execute();
};

