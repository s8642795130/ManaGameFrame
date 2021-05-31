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
private:
	std::string m_server_name;
protected:
	bool LoadPluginLibrary(const std::string& pluginDLLName);
	void RegisterAll();
public:
	void LoadAllPluginLibrary(std::vector<std::shared_ptr<PluginData>> vec_plugin);

	// config
	void SetServerName(const std::string& SetServerName);
	virtual const std::string& GetServerName();

	// runtime
	void Init();
	void AfterInit();
	void ReadyExecute();
	void Execute();
};

