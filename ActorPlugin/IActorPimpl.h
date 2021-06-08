#pragma once
#include <memory>
#include <vector>
#include <string>

class IActorMsg;
class IPluginManager;

class IActorPimpl
{
public:
	virtual void SendMsgToActor(std::unique_ptr<IActorMsg>& actor_msg) = 0;
	virtual void RPCMsg() = 0;
	virtual void ResponseMsg(const std::string& uid, const std::vector<char>& stream) = 0;
	virtual void NotifyMsg(const std::string& uid, const std::vector<char>& stream) = 0;
	virtual void BroadcastMsg() = 0;
};