#pragma once
#include <string>
#include <functional>
#include <memory>

#include "IApplication.h"
#include "ConfigFile.h"
#include "ServerController.h"
#include "ThreadPool.h"
#include "ServerNet.h"
#include "ClientNet.h"
#include "PluginManager.h"

class Application : public IApplication
{
private:
	std::shared_ptr<ConfigFile> m_config_file;
	std::unique_ptr<ServerController> m_server_controller;
	std::shared_ptr<ThreadPool> m_thread_pool;
	std::unique_ptr<ServerNet> m_server_net;
	std::unique_ptr<PluginManager> m_plugin_manager;
public:
	Application();
	// NetPlugin (bind net msg)
	// ServerNet_ClientPlugin (client list)
	// ActorPlugin
	// ConfigPlugin
	// MessageDefinePlugin
	// ServerControlPlugin
	virtual void AddReceiveCallBack(const int msg_id, std::function<void(ClientDescriptor*)> call_func);
	virtual void SendMsgToActor(std::unique_ptr<IActorMsg>& actor_msg); // need call
	virtual void RPCMsg(); // need call
	virtual void ResponseMsg(); // need call
	virtual void PushMsg(); // need call
	virtual void BroadcastMsg(); // need call
	virtual void AddActorToThreadCell(std::shared_ptr<IActor> ptr_actor);
	virtual void RemoveActorFromThreadCell(const std::string& uuid);
	virtual std::shared_ptr<IConfigFile> GetConfigPtr();
	//
	void LoadConfig(const std::string& server_name);
	void StartLoadAllLibrary();
	//
	void StartConnectServer();
	//
	void LibInit();
	void LibAfterInit();
	void LibReadyExecute();
	void LibExecute();
	//
	void StartThreadPool();
	void StartNetwork();
	void StartNetEventLoop();
};

