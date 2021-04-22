#pragma once
#include "ClientNet.h"
class MasterObj : public ClientNet
{
public:
	MasterObj() = default;
	~MasterObj() = default;

	void ConnectMaster();
	void SendOnline();
};

