#pragma once
#include "../Server/Actor.h"
#include "../Server/ClientNet.h"

class ClientNetControlActor : public Actor
{
public:
	ClientNetControlActor() : Actor()
	{
	}

	virtual ~ClientNetControlActor()
	{
	}

	void ProcClientMessage(ClientNet* ptr_client)
};

