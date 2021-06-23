#pragma once
#include <map>
#include "../ActorPlugin/Actor.h"
#include "../ServerNetPlugin/IClientNetActor.h"

class MasterActor : public Actor
{
private:
	std::map<std::string, std::string> m_map_online_server; // key: server_name, value: uuid
protected:
	void ServerOnline(const std::string server_name, const std::string uuid);
public:
	MasterActor(std::shared_ptr<IPluginManager> ptr_manager) : Actor(ptr_manager)
	{

	}

	// interface
	void OnServerOnlineCallback(IClientNetActor& client);
};

