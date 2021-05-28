#pragma once

#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <map>
#include <functional>
#include <memory>
#include <cstring>

#include "DefineHeader.h"
#include "ThreadRouter.h"
#include "ActorMsg.h"
#include "MessageDefine.h"

#include "../Server/ByteBuffer.h"
#include "../ActorPlugin/IActor.h"

/*
	This is a base client descriptor and virtual methods should be implemented by a derived class.
	Returning false from any of the methods will result in Cleanup() being called and the client
	descriptor being deconstructed immediately.
*/
class IClientNetActor : public IActor
{
protected:
	std::string m_uid;
public:
	IClientNetActor() = default;
	virtual ~IClientNetActor() = default;

	// member
	int m_client_fd = 0;
	sockaddr_in m_client_sin = { 0 };

	//called when a client fd becomes available for writing
	virtual bool ReadReady() { throw std::runtime_error("ReadReady() not implemented"); }

	//called when a client fd becomes available for reading
	virtual bool WriteReady() { throw std::runtime_error("WriteReady() not implemented"); }

	//called periodically to check if fd is still alive (used to implement timeout)
	virtual bool HeartBeat() { throw std::runtime_error("HeartBeat() not implemented"); }

	//called when the server is done with the client and the fd should be closed
	virtual void ServerClose() { throw std::runtime_error("ServerClose() not implemented"); }

	//called if the connection was forcibly closed by the client
	virtual void ClientClose() { throw std::runtime_error("ClientClose() not implemented"); }

	//client's unique id
	const int GetSid() { return m_client_fd; }

	// client uuid
	const std::string& GetUid() const
	{
		return m_uid;
	}

	virtual std::shared_ptr<ByteBuffer>& GetBuffer() = 0;
};