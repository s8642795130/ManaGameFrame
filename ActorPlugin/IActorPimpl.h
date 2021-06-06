#pragma once
#include <memory>

class IActorMsg;
class IPluginManager;

class IActorPimpl
{
private:
	static std::shared_ptr<IActorPimpl> m_pimpl;
public:
	static std::shared_ptr<IActorPimpl> GetActorPimpl()
	{
		return m_pimpl;
	}

	static void SetActorPimpl(std::shared_ptr<IActorPimpl> pimpl)
	{
		m_pimpl = pimpl;
	}

	virtual void SendMsgToActor(std::unique_ptr<IActorMsg>& actor_msg) = 0;
	virtual void RPCMsg() = 0;
	virtual void ResponseMsg(const std::string& uid, const std::vector<char>& stream) = 0;
	virtual void NotifyMsg(const std::string& uid, const std::vector<char>& stream) = 0;
	virtual void BroadcastMsg() = 0;
};