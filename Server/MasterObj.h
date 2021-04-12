#pragma once
#include "Client.h"
class MasterObj : public Client
{
public:
	MasterObj(std::shared_ptr<std::map<int, std::function<void(ClientDescriptor*, int, char*)>>>& receive_callBack) : Client(receive_callBack)
	{

	}

	void ConnectMaster();
};

