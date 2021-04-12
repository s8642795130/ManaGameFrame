#pragma once

#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <unistd.h>

#include "ClientDescriptor.h"

class Client : public ClientDescriptor
{
protected:
	time_t m_last_active;
public:
	Client(std::shared_ptr<std::map<int, std::function<void(ClientDescriptor*, int, char*)>>>& receive_callBack) : ClientDescriptor(receive_callBack)
	{

	}

	virtual bool ReadReady();
	virtual bool WriteReady();
	virtual bool HeartBeat();
	virtual void ClientClose();
	virtual void ServerClose();
	//
	virtual void SendData(const int& value);
};