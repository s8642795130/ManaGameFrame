#pragma once

#include <iostream>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <unistd.h>

#include "MessageDefine.h"
#include "ClientDescriptor.h"

class ClientNet : public ClientDescriptor
{
protected:
	// Client Type
	time_t m_last_active;
public:
	ClientNet()
	{

	}

	virtual bool ReadReady()
	{
		std::array<char, DEFAULT_BUFLEN> buffer;
		size_t bytes_read = 0;

		//we must drain the entire read buffer as we won't get another event until client sends more data
		while (true)
		{
			bytes_read = recv(m_fd, buffer.data(), size_t(1024), 0);
			if (bytes_read <= 0)
			{
				break;
			}

			Parsing(buffer, bytes_read);
			// data_buffer.append(buffer, bytes_read);
		}

		//client triggered EPOLLIN but sent no data (usually due to remote socket being closed)

		// std::cout << "[i] client %s:%d said: %s\n", inet_ntoa(client_addr_), client_port_, data_buffer.c_str());

		//update last active time to prevent timeout
		m_last_active = time(0);

		return true;
	}

	virtual bool WriteReady()
	{
		/*
			during heavy network I/O fds can become unwritable and subsequent calls to write() / send() will fail,
			in this case the data which failed to send should be stored in a buffer and the operation should be
			retried when WriteReady() is called to signal the fd is writable again (this is up to you to implement).
		*/
		return true;
	}

	virtual bool HeartBeat()
	{
		// if no operations occurred during timeout period return false to signal server to close socket
		if (static_cast<time_t>(m_last_active + m_timeout) < time(0))
		{
			std::cout << "[i] connection " << inet_ntoa(m_client_sin.sin_addr) << ":" << ntohs(m_client_sin.sin_port) << " has timed out" << std::endl;
			return false;
		}

		return true;
	}

	virtual void ClientClose()
	{
		close(m_fd);

		std::cout << "[-] connection " << inet_ntoa(m_client_sin.sin_addr) << ":" << ntohs(m_client_sin.sin_port) << " closed by client" << std::endl;
	}

	virtual void ServerClose()
	{
		close(m_fd);

		std::cout << "[-] connection " << inet_ntoa(m_client_sin.sin_addr) << ":" << ntohs(m_client_sin.sin_port) << " closed by server" << std::endl;
	}
};