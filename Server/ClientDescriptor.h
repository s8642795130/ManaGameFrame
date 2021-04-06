#pragma once

#include <netinet/in.h>
#include <stdexcept>
#include <string>


/*
	This is a base client descriptor and virtual methods should be implemented by a derived class.
	Returning false from any of the methods will result in Cleanup() being called and the client
	descriptor being deconstructed immediately.
*/
class ClientDescriptor
{
protected:
	const int m_timeout = 30;
	int m_fd = -1;
	in_addr m_client_addr;
	uint16_t m_client_port;
	std::string m_uid;
public:
	ClientDescriptor()
	{

	}

	virtual ~ClientDescriptor()
	{

	}

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
	const int GetSid() { return m_fd; }

	// client uuid
	const std::string& GetUid() const
	{
		return m_uid;
	}
};