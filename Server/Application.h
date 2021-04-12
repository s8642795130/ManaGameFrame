#pragma once
#include <string>
#include <functional>
#include <memory>

#include "IApplication.h"
#include "ThreadPool.h"
#include "ServerNet.h"
#include "Client.h"

class Application : public IApplication
{
private:
	std::unique_ptr<ThreadPool> m_thread_pool;
	std::unique_ptr<ServerNet<Client>> m_server_net;
public:
	Application();
	virtual void AddReceiveCallBack(const int msg_id, std::function<void(ClientDescriptor*, int, char*)> call_func);
	virtual void SendMsgToActor(const std::string& sender_uuid, const std::string& receiver_uuid, std::function<void(void)> call_func, std::function<void(void)> callback);
	virtual void RpcCall();
	virtual void ResponseMsg();
	virtual void PushMsg();
	virtual void BroadcastMsg();
	virtual void AddActorToThreadCell(std::shared_ptr<IActor> ptr_actor);
	virtual void RemoveActorToThreadCell(const std::string& uuid);
	//
	void StartThreadPool();
	void StartNetwork();
};

