#pragma once
#include <string>
#include <map>
#include <memory>

#include "IPlugin.h"
#include "DynLib.h"
#include "IPluginManager.h"

struct PluginData;

class PluginManager : public IPluginManager
{
public:
	void LoadAllPluginLibrary(std::vector<std::shared_ptr<PluginData>> vec_plugin);
	bool LoadPluginLibrary(const std::string& pluginDLLName);
	void RegisterAll();

	// runtime
	void Init();
	void AfterInit();
	void ReadyExecute();
	void Execute();
};

