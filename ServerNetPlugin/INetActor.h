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

	virtual bool PushData(const BYTE* ptr_data, int length) = 0;

	virtual std::shared_ptr<ByteBuffer>& GetBuffer() = 0;

	virtual void SendStream(const std::vector<char> stream) = 0;

	virtual const std::string& GetUid() const = 0;
	virtual void ProcessIO() = 0;
};