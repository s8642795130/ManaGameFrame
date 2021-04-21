#pragma once
#include "../Server/Actor.h"

class MasterActor : public Actor
{
public:
	MasterActor() : Actor()
	{

	}

	virtual ~MasterActor()
	{

	}

	void ServerOnline(int test, int c);
};

