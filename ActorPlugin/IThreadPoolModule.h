#pragma once
#include <memory>

#include "../Server/IModule.h"

class IActor;
class IActorMsg;

class IThreadPoolModule : public IModule
{
public:
	IThreadPoolModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	// function
	virtual void AddActorToThreadCell(std::shared_ptr<IActor> ptr_actor)
	{
	}

	virtual void AddActorMsgToThreadCell(std::unique_ptr<IActorMsg>& ptr_actor_msg)
	{
	}

	virtual void RemoveActorFromThreadCell(const std::string& uuid)
	{
	}
};