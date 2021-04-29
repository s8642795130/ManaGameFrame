#pragma once
#include "Actor.h"
#include "ClientNet.h"

class ClientNetControlActor : public Actor
{
public:
	ClientNetControlActor() : Actor()
	{
	}

	virtual ~ClientNetControlActor()
	{
	}

	void ProcClientMessage(ClientNet* ptr_client);
};

