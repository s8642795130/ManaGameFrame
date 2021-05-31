#pragma once
#include <map>
#include "../ActorPlugin/Actor.h"

class MasterActor : public Actor
{
private:
	std::map<std::string, std::string> m_map_online_server; // key: server_name, value: uuid
public:
	MasterActor() : Actor()
	{

	}

	virtual ~MasterActor()
	{

	}

	void ServerOnline(const std::string server_name, const std::string uuid);
};

