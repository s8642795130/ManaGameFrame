#pragma once
#include <memory>
#include <vector>
#include <string>

class IActorMsg;
class IPluginManager;

class IActorPimpl
{
public:
	// interface
	virtual void SendMsgToActor(std::unique_ptr<IActorMsg>& actor_msg) = 0;
	virtual void RPCMsg(const int major_id, const int minor_id, const std::vector<char>& stream, const std::string& router = "") = 0;
	virtual void ResponseMsg(const std::string& uid, const std::vector<char>& stream) = 0;
	virtual void NotifyMsg(const std::string& uid, const std::vector<char>& stream) = 0;
	virtual void BroadcastMsg(const std::vector<std::string>& vec_uid, const std::vector<char>& stream) = 0;
};