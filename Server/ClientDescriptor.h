#pragma once

#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <map>
#include <functional>
#include <memory>
#include <cstring>

#include "DefineHeader.h"
#include "ByteBuffer.h"

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
	std::string m_uid;
	//
	std::shared_ptr<std::map<int, std::function<void(ClientDescriptor*, int, char*)>>> m_receive_callBack;
	std::shared_ptr<ByteBuffer> m_buffer;
public:
	ClientDescriptor(std::shared_ptr<std::map<int, std::function<void(ClientDescriptor*, int, char*)>>>& receive_callBack)
	{
		m_receive_callBack = receive_callBack;
	}

	virtual ~ClientDescriptor()
	{

	}

	// member
	int m_client_fd = 0;
	sockaddr_in m_client_sin = {0};

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

	// ProccessIO
	virtual void ProccessIO() = 0;

	//client's unique id
	const int GetSid() { return m_fd; }

	// client uuid
	const std::string& GetUid() const
	{
		return m_uid;
	}

	/// <summary>
	/// GetBuffer
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<ByteBuffer>& GetBuffer()
	{
		return m_buffer;
	}

	/// <summary>
	/// Parsing
	/// </summary>
	/// <param name="buffer"></param>
	/// <param name="len"></param>
	void Parsing(std::array<char, DEFAULT_BUFLEN>& buffer, std::size_t len)
	{
		auto buf_remain_len = len;
		if (m_buffer->GetHeaderStatus() == false)
		{
			// is not read length
			auto remain_len = m_buffer->GetRemainLen();
			auto len_buf = remain_len < len ? remain_len : len;

			//
			m_buffer->RecvHeader(buffer.data(), len_buf);
			buf_remain_len -= len_buf;

			//
			if (m_buffer->GetRemainLen() == 0)
			{
				m_buffer->ResetHeader();
			}
		}

		if (buf_remain_len != 0)
		{
			//
			auto unreceived_len = m_buffer->GetUnreceivedLen();
			auto push_data_len = unreceived_len < buf_remain_len ? unreceived_len : buf_remain_len;
			m_buffer->PushData(buffer.data() + (len - buf_remain_len), push_data_len);
			buf_remain_len -= push_data_len;
		}

		if (m_buffer->GetRemainLen() == 0)
		{
			// io
			ProccessIO();
		}

		if (buf_remain_len != 0)
		{
			std::array<char, DEFAULT_BUFLEN> buf;
			std::memcpy(buf.data(), buffer.data() + (len - buf_remain_len), buf_remain_len);
			Parsing(buf, buf_remain_len);
		}
	}
};