#pragma once
#include "../ActorPlugin/Actor.h"
#include "../ServerNetPlugin/INetActor.h"

class LoginActor : public Actor
{
public:
	LoginActor(std::shared_ptr<IPluginManager> ptr_manager) : Actor(ptr_manager)
	{
	}

	void OnLoginOnlineCallback(std::shared_ptr<INetActor> ptr_client, std::string uuid, std::string path, std::string query);
};

