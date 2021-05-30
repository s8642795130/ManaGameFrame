#pragma once
#include "IActorPimpl.h"
#include "IThreadPoolModule.h"

class ActorPimpl : public IActorPimpl
{
protected:
	std::shared_ptr<IThreadPoolModule> m_thread_pool_module;
public:
	ActorPimpl(std::shared_ptr<IPluginManager> ptr)
	{
		m_thread_pool_module = ptr->GetModule<IThreadPoolModule>();
	}

	virtual void SendMsgToActor(std::unique_ptr<IActorMsg>& actor_msg)
	{
		m_thread_pool_module->AddActorMsgToThreadCell(actor_msg);
	}

	virtual void RPCMsg()
	{

	}
	virtual void ResponseMsg()
	{

	}

	virtual void PushMsg()
	{

	}

	virtual void BroadcastMsg()
	{

	}
};