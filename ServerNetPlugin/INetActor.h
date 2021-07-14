#pragma once
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <unistd.h>

#include "GlobalDef.h"
#include "../Server/ByteBuffer.h"
#include "../ActorPlugin/Actor.h"

class INetActor : public Actor
{
public:
	INetActor(std::shared_ptr<IPluginManager> ptr_manager) : Actor(ptr_manager)
	{
	}

	//client's unique id
	virtual const unsigned long GetSid() const = 0;
	
	virtual void ClientClose() = 0;

	virtual void PushData(const BYTE* ptr_data, int length) = 0;

	virtual std::shared_ptr<ByteBuffer>& GetBuffer() = 0;

	virtual void SendStream(const std::vector<char> stream) = 0;
	virtual void SendData(const int major, const int minor, std::vector<char> value) = 0;

	virtual void ProcessIO() = 0;
};