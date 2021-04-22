#pragma once
#include <string>
#include <functional>
#include <memory>

#include "IApplication.h"
#include "ThreadPool.h"
#include "ServerNet.h"
#include "ClientNet.h"
#include "PluginManager.h"

class Application : public IApplication
{
private:
	std::unique_ptr<ThreadPool> m_thread_pool;
	// std::unique_ptr<ServerNet<ClientNet>> m_server_net;
	std::unique_ptr<ServerNet> m_server_net;
	std::unique_ptr<PluginManager> m_plugin_manager;
public:
	Application();
	virtual void AddReceiveCallBack(const int msg_id, std::function<void(ClientDescriptor*)> call_func);
	virtual void SendMsgToActor(std::unique_ptr<IActorMsg>& actor_msg);
	virtual void RpcCall();
	virtual void ResponseMsg();
	virtual void PushMsg();
	virtual void BroadcastMsg();
	virtual void AddActorToThreadCell(std::shared_ptr<IActor> ptr_actor);
	virtual void RemoveActorToThreadCell(const std::string& uuid);
	//
	void LoadConfig();
	void StartLoadAllLibrary(int test_code);
	//
	void LibInit();
	void LibAfterInit();
	void LibReadyExecute();
	void LibExecute();
	//
	void StartThreadPool();
	void StartNetwork(int test_code);
	void StartNetEventLoop();
	void ConnectMaster();
};

