#pragma once
#include "../ActorPlugin/Actor.h"
#include "IClientNetActor.h"

class ServerObjActor : public Actor
{
public:
	ServerObjActor(std::shared_ptr<IPluginManager> ptr_manager) : Actor(ptr_manager)
	{

	}

	virtual ~ServerObjActor()
	{

	}

	void OnServerOnlineCallback(IClientNetActor& ptr_client);
};

