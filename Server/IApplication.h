#pragma once
#include <string>
#include <functional>
#include <memory>

#include "IActor.h"

class IApplication
{
protected:
	static std::shared_ptr<IApplication> m_app;
public:
	static std::shared_ptr<IApplication> GetPtr()
	{
		return m_app;
	}

	virtual void AddReceiveCallBack(const int msgID, void* pBase, std::function<void(void)> call_func) = 0;
	virtual void SendMsgToActor(const std::string& sender_uuid, const std::string& receiver_uuid, std::function<void(void)> call_func, std::function<void(void)> callback) = 0;
	virtual void RpcCall() = 0;
	virtual void ResponseMsg() = 0;
	virtual void PushMsg() = 0;
	virtual void BroadcastMsg() = 0;
	virtual void AddActorToThreadCell(std::shared_ptr<IActor> ptr_actor) = 0;
	virtual void RemoveActorToThreadCell(const std::string& uuid) = 0;
};
