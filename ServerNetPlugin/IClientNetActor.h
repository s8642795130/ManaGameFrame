#pragma once

#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <map>
#include <functional>
#include <memory>
#include <cstring>

#include "../Server/ByteBuffer.h"
#include "../ActorPlugin/Actor.h"

/*
	This is a base client descriptor and virtual methods should be implemented by a derived class.
	Returning false from any of the methods will result in Cleanup() being called and the client
	descriptor being deconstructed immediately.
*/
class IClientNetActor : public Actor, public std::enable_shared_from_this<IClientNetActor>
{
public:
	IClientNetActor() = default;
	virtual ~IClientNetActor() = default;

	// member
	int m_client_fd = 0;
	sockaddr_in m_client_sin = { 0 };

	// connect server
	virtual bool ConnectServer(const std::string& ip, const int port) = 0;

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

	// client uid
	virtual void SetUid(const std::string& uid) = 0;
	virtual const std::string& GetUid() const = 0;

	// client data
	virtual const std::map<std::string, std::string> GetClientData() const = 0;
	virtual void UpdateClientData(const std::string& key, const std::string& value, int type) = 0;

	virtual std::shared_ptr<ByteBuffer>& GetBuffer() = 0;

	virtual void SendData(const int major, const int minor, std::vector<char> value) = 0;
	// virtual void SendBuffer(std::shared_ptr<ByteBuffer> buffer) = 0;
	virtual void SendStream(std::vector<char> stream) = 0;
};