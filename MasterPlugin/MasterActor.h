#pragma once
#include <map>
#include "../ActorPlugin/Actor.h"
#include "../ServerNetPlugin/INetActor.h"

class MasterActor : public Actor
{
private:
	std::map<std::string, std::string> m_map_online_server; // key: server_name, value: uuid
protected:
	void ServerOnline(const std::string& server_name, const std::string& online_server_uuid);
	void BroadcastServerOnline(const std::string& server_name);
	void SendCurrentAllServerToOnlineServer(const std::string& online_server_uuid);
public:
	MasterActor(std::shared_ptr<IPluginManager> ptr_manager) : Actor(ptr_manager)
	{

	}

	// interface
	void OnServerOnlineCallback(std::shared_ptr<INetActor> ptr_client);
};

