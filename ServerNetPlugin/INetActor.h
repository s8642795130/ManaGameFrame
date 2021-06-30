#pragma once
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <unistd.h>

#include "../ActorPlugin/Actor.h"
#include "../Server/ByteBuffer.h"

class INetActor : public Actor
{
protected:
	int m_client_fd = 0;
	sockaddr_in m_client_sin = { 0 };
	std::shared_ptr<ByteBuffer> m_buffer;
public:
	// interface
	bool ConnectServer(const std::string& ip, const int port)
	{
		bool ret = true;

		m_client_fd = socket(AF_INET, SOCK_STREAM, 0);

		// test code
		int on = 1;
		setsockopt(m_client_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

		// sockaddr_in
		m_client_sin.sin_family = AF_INET;
		m_client_sin.sin_port = htons(static_cast<uint16_t>(port));
		m_client_sin.sin_addr.s_addr = inet_addr(ip.c_str());

		if (connect(m_client_fd, (struct sockaddr*)&m_client_sin, sizeof(m_client_sin)) < 0)
		{
			fprintf(stderr, "socket connect error = %d ( %s )\n", errno, strerror(errno));
			ret = false;
		}

		return ret;
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

			OnParsing(buffer, bytes_read);
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

	virtual void OnParsing(std::array<char, DEFAULT_BUFLEN>& buffer, ssize_t len) = 0;
	virtual void ProcessIO() = 0;
};