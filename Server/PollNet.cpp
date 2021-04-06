#include <iostream>
#include <algorithm>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#include "PollNet.h"

void Usge(const char* proc)
{
	std::cout << proc << "[local_ip] [local_port]" << std::endl;
}

int array[500];

int PollNet::InitSocket(std::string& ip, std::uint16_t port)
{
	// int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		std::perror("socket error");
		std::exit(1);
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip.c_str());

	bind(sock, (struct sockaddr*)&local, sizeof(local));
	listen(sock, SOMAXCONN);
	return sock;
}

void PollNet::StartPoll(std::string& ip, std::uint16_t port)
{
	m_listen_socket = InitSocket(ip, port);

	m_arr_poll_fd[0].fd = m_listen_socket;
	m_arr_poll_fd[0].events = POLLIN;

	int ret = 0;
	int timeout = -1;

	std::for_each(std::begin(m_arr_poll_fd), std::end(m_arr_poll_fd), [](struct pollfd& item) -> void
		{
			item.fd = -1;
		}
	);

	while (true)
	{
		switch (ret = poll(m_arr_poll_fd.data(), m_nfds, timeout))
		{
		case 0:
			std::cout << "timeout..." << std::endl;
			break;
		case -1:
			std::cout << "poll error" << std::endl;
			break;
		default:
		{
			if (m_arr_poll_fd[0].revents & POLLIN)
			{
				HandleConnect();
			}

			//
			for (int i = 1; i < m_nfds; i++)
			{
				if (m_arr_poll_fd[i].revents & POLLIN)
				{
					HandleRead(m_arr_poll_fd[i]);
				}

				if (m_arr_poll_fd[i].revents & POLLOUT)
				{
					// char* msg = "duanzhihong";
					// write(m_arr_poll_fd[i].fd, msg, strlen(msg));
					// m_arr_poll_fd[i].events = POLLIN;
				}
			}
		}
		break;
		}
	}
}

bool PollNet::HandleConnect()
{
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	int new_sock = accept(m_listen_socket, (struct sockaddr*)&client, &len);

	std::cout << "accpet finish: " << new_sock << std::endl;
	std::cout << "get a new client" << std::endl;

	for (int j = 1; j < m_max_poll_count; j++)
	{
		if (m_arr_poll_fd[j].fd < 0)
		{
			m_arr_poll_fd[j].fd = new_sock;
			m_arr_poll_fd[j].events = POLLIN;
			if (j + 1 > m_nfds)
			{
				m_nfds = j + 1;
			}
			break;
		}
	}

	return true;
}

bool PollNet::HandleRead(struct pollfd& poll_fd)
{
	char buf[1024];
	ssize_t s = read(poll_fd.fd, buf, sizeof(buf) - 1);
	if (s > 0)
	{
		buf[s] = 0;
		fflush(stdout);
		poll_fd.events = POLLOUT;
	}
	else
	{
		close(poll_fd.fd);
		poll_fd.fd = -1;
	}
	return true;
}