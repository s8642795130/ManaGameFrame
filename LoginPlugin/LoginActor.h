#pragma once
#include "../ActorPlugin/Actor.h"

class IClientNetActor;

class LoginActor : public Actor
{
public:
	LoginActor(std::shared_ptr<IPluginManager> ptr_manager) : Actor(ptr_manager)
	{

	}

	void OnLoginOnlineCallback(IClientNetActor& client);
};

