#pragma once
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <unistd.h>

#include "../ActorPlugin/Actor.h"
#include "../Server/ByteBuffer.h"
#include "ClientPimpl.h"

class INetActor : public Actor
{
protected:

	// impl
	std::shared_ptr<ClientPimpl> m_client_impl;
	//
	std::shared_ptr<ByteBuffer> m_buffer;
public:

	int m_client_fd = 0;
	sockaddr_in m_client_sin = { 0 };

	INetActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr_impl) :
		Actor(ptr_manager),
		m_client_impl(ptr_impl),
		m_buffer(std::make_shared<ByteBuffer>())
	{
	}

	//client's unique id
	const int GetSid()
	{
		return m_client_fd;
	}
	
	void ClientClose()
	{
		close(m_client_fd);

		std::cout << "[-] connection " << inet_ntoa(m_client_sin.sin_addr) << ":" << ntohs(m_client_sin.sin_port) << " closed by client" << std::endl;
	}

	virtual bool OnReadReady()
	{
		bool ret = true;

		std::array<char, DEFAULT_BUFLEN> buffer;
		ssize_t bytes_read = 0;

		//we must drain the entire read buffer as we won't get another event until client sends more data
		while (true)
		{
			// bytes_read = recv(m_fd, buffer.data(), DEFAULT_BUFLEN, 0);
			bytes_read = read(m_client_fd, buffer.data(), DEFAULT_BUFLEN);

			// if (bytes_read == -1)
			if (bytes_read <= 0)
			{
				if (errno != EAGAIN)
				{
					std::perror("read error");
					ret = false;
				}
				break;
			}

			Parsing(buffer, bytes_read);
			// data_buffer.append(buffer, bytes_read);
		}

		// client triggered EPOLLIN but sent no data (usually due to remote socket being closed)

		// std::cout << "[i] client %s:%d said: %s\n", inet_ntoa(client_addr_), client_port_, data_buffer.c_str());

		// update last active time to prevent timeout
		// m_last_active = time(0);

		return ret;
	}

	virtual bool OnWriteReady()
	{
		/*
		during heavy network I/O fds can become unwritable and subsequent calls to write() / send() will fail,
		in this case the data which failed to send should be stored in a buffer and the operation should be
		retried when WriteReady() is called to signal the fd is writable again (this is up to you to implement).
		*/
		std::cout << "WriteReady()" << std::endl;
		return true;
	}
	virtual bool OnHeartBeat()
	{
		// if no operations occurred during timeout period return false to signal server to close socket
		//if (static_cast<time_t>(m_last_active + m_timeout) < time(0))
		//{
			//std::cout << "[i] connection " << inet_ntoa(m_client_sin.sin_addr) << ":" << ntohs(m_client_sin.sin_port) << " has timed out" << std::endl;
			//return false;
		//}

		return true;
	}
	virtual void OnServerClose()
	{
		std::cout << "[-] server close " << inet_ntoa(m_client_sin.sin_addr) << ":" << ntohs(m_client_sin.sin_port) << " closed by client" << std::endl;
	}

	virtual void Parsing(std::array<char, DEFAULT_BUFLEN>& buffer, ssize_t len)
	{
		auto buf_remain_len = len;
		if (m_buffer->GetHeaderStatus() == false)
		{
			// is not read length
			auto remain_len = m_buffer->GetRemainingLen();
			auto len_buf = remain_len < len ? remain_len : len;

			//
			m_buffer->RecvHeader(buffer.data(), len_buf);
			buf_remain_len -= len_buf;

			//
			if (m_buffer->GetRemainingLen() == 0)
			{
				if (m_buffer->ResetHeader() == false)
				{
					return;
				}

				if (m_buffer->GetRemainingLen() == 0)
				{
					// io actor
					ProcessIO();

					// reset msg data
					m_buffer->ResetData();
					if (buf_remain_len != 0)
					{
						std::array<char, DEFAULT_BUFLEN> buf;
						std::memcpy(buf.data(), buffer.data() + (len - buf_remain_len), buf_remain_len);
						Parsing(buf, buf_remain_len);
						return;
					}
				}
			}
		}

		if (buf_remain_len != 0)
		{
			//
			auto unreceived_len = m_buffer->GetRemainingLen();
			auto push_data_len = unreceived_len < buf_remain_len ? unreceived_len : buf_remain_len;
			m_buffer->PushData(buffer.data() + (len - buf_remain_len), push_data_len);
			buf_remain_len -= push_data_len;
		}

		if (m_buffer->GetRemainingLen() == 0)
		{
			// io
			ProcessIO();

			// reset msg data
			m_buffer->ResetData();
			if (buf_remain_len != 0)
			{
				std::array<char, DEFAULT_BUFLEN> buf;
				std::memcpy(buf.data(), buffer.data() + (len - buf_remain_len), buf_remain_len);
				Parsing(buf, buf_remain_len);
			}
		}
	}

	virtual void SendStream(std::vector<char> stream) final
	{
		send(m_client_fd, stream.data(), stream.size(), 0);
	}

	virtual const std::string& GetUid() const = 0;
	virtual void ProcessIO() = 0;
};