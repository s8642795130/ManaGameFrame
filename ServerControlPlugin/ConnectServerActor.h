#pragma once
#include <string>
#include "../Server/Actor.h"

class ConnectServerActor : public Actor
{
public:
	ConnectServerActor() : Actor()
	{

	}

	virtual ~ConnectServerActor()
	{

	}

	void ConnectServer(std::string& ip, int port);
};

