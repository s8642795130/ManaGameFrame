#pragma once
#include <string>
#include <functional>
#include <memory>

//#include "IActor.h"
//#include "IActorMsg.h"
//#include "ClientDescriptor.h"

class IActor;
class ClientDescriptor;
class ClientNet;
class IActorMsg;
class IConfigFile;

class IApplication
{
protected:
	static std::shared_ptr<IApplication> m_app;
public:
	static std::shared_ptr<IApplication> GetPtr()
	{
		return m_app;
	}

	static void SetPtr(std::shared_ptr<IApplication> app)
	{
		m_app = app;
	}

	virtual void AddReceiveCallBack(const int msgID, std::function<void(ClientDescriptor*)> call_func) = 0;
	virtual void SendMsgToActor(std::unique_ptr<IActorMsg>& actor_msg) = 0;
	virtual void RPCMsg() = 0;
	virtual void ResponseMsg() = 0;
	virtual void PushMsg() = 0;
	virtual void BroadcastMsg() = 0;
	virtual void AddActorToThreadCell(std::shared_ptr<IActor> ptr_actor) = 0;
	virtual void RemoveActorFromThreadCell(const std::string& uuid) = 0;
	virtual std::shared_ptr<IConfigFile> GetConfigPtr() = 0;
};
