#pragma once
#include "ClientNet.h"
class MasterObj : public ClientNet
{
public:
	MasterObj(std::shared_ptr<std::map<int, std::function<void(ClientDescriptor*, int, char*)>>>& receive_callBack) : ClientNet(receive_callBack)
	{

	}

	void ConnectMaster();
};

