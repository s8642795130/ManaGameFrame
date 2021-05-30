#pragma once
#include "IServerObjModule.h"

class ServerObjModule : public IServerObjModule
{
private:
	std::map<std::string, std::string> m_map_server; // key: server_name, value: uuid
public:
	virtual const std::map<std::string, std::string>& GetServerMap();
};

