#pragma once
#include <memory>

class IActor;
class IActorMsg;

class IThreadPool
{
public:
	virtual void AddActorToThreadCell(std::shared_ptr<IActor> ptr_actor) = 0;
	virtual void AddActorMsgToThreadCell(std::unique_ptr<IActorMsg>& ptr_actor_msg) = 0;
	virtual void RemoveActorFromThreadCell(const std::string& uuid) = 0;
};