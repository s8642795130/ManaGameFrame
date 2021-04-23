#pragma once
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/sendfile.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <errno.h>

#include <array>
#include <iostream>

#include "DefineHeader.h"

class ServerNetTest
{
private:
	int m_epfd = 0;
	int m_listenfd = 0;
	struct epoll_event m_ev, m_events[WORKER_MAX_EVENTS];

	void setnonblocking(int fd)
	{
		int opts;

		opts = fcntl(fd, F_GETFL);
		if (opts < 0)
		{
			perror("fcntl(F_GETFL)\n");
			exit(1);
		}
		opts = (opts | O_NONBLOCK);
		if (fcntl(fd, F_SETFL, opts) < 0)
		{
			perror("fcntl(F_SETFL)\n");
			exit(1);
		}
	}

public:
	void StartNetwork(int port)
	{
		// struct epoll_event ev, events[WORKER_MAX_EVENTS];
		struct sockaddr_in local;

		//listen socket
		if ((m_listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("sockfd\n");
			exit(1);
		}
		setnonblocking(m_listenfd);
		bzero(&local, sizeof(local));
		local.sin_family = AF_INET;
		local.sin_addr.s_addr = htonl(INADDR_ANY);
		local.sin_port = htons(port);
		if (bind(m_listenfd, (struct sockaddr*)&local, sizeof(local)) < 0)
		{
			perror("bind\n");
			exit(1);
		}
		listen(m_listenfd, SOMAXCONN);

		// epfd = epoll_create(MAX_EVENTS);
		m_epfd = epoll_create1(0);
		if (m_epfd == -1)
		{
			perror("epoll_create");
			exit(EXIT_FAILURE);
		}
		m_ev.events = EPOLLIN;
		m_ev.data.fd = m_listenfd;
		if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_listenfd, &m_ev) == -1)
		{
			perror("epoll_ctl: listen_sock");
			exit(EXIT_FAILURE);
		}

		// close(epfd);
		// close(listenfd);
	}

	void EventLoop()
	{
		int nfds = 0;
		int sockfd = 0;
		int connfd = 0;
		std::array<char, BUFSIZ> buf;
		struct sockaddr_in remote;
		socklen_t addrlen;
		ssize_t nread, n;

		for (;;)
		{
			nfds = epoll_wait(m_epfd, m_events, WORKER_MAX_EVENTS, -1);
			if (nfds == -1)
			{
				perror("epoll_wait error");
				exit(EXIT_FAILURE);
			}

			for (int i = 0; i < nfds; ++i)
			{
				sockfd = m_events[i].data.fd;
				if (sockfd == m_listenfd)
				{
					while ((connfd = accept(m_listenfd, (struct sockaddr*)&remote, &addrlen)) > 0)
					{
						char* ipaddr = inet_ntoa(remote.sin_addr);
						printf("accept a connection from [%s]\n", ipaddr);
						setnonblocking(connfd);	//
						m_ev.events = EPOLLIN | EPOLLET;	//
						m_ev.data.fd = connfd;
						if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, connfd, &m_ev) == -1)
						{
							perror("epoll_ctl: add");
							exit(EXIT_FAILURE);
						}
					}
					if (connfd == -1)
					{
						if (errno != EAGAIN && errno != ECONNABORTED && errno != EPROTO && errno != EINTR)
							perror("accept");
					}
					continue;
				}
				if (m_events[i].events & EPOLLIN)
				{
					std::cout << "got EPOLLIN" << std::endl;
					n = 0;
					while ((nread = read(sockfd, buf.data() + n, BUFSIZ - 1)) > 0)
					{
						std::cout << "got recv length: " << nread << std::endl;
						n += nread;
					}
					if (nread == -1 && errno != EAGAIN)
					{
						perror("read error");
						continue;
					}
					printf("recv from client data [%s]\n", buf);

					// write(sockfd, "test-123", sizeof("test-123"));
					/*
					ev.data.fd = sockfd;
					ev.events = events[i].events | EPOLLOUT;
					if (epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev) == -1)
					{
						perror("epoll_ctl: mod");
					}
					*/
				}
				if (m_events[i].events & EPOLLOUT)
				{
					snprintf(buf.data(), buf.size(), "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\nHello World", 11);
					ssize_t nwrite, data_size = buf.size();
					n = data_size;
					while (n > 0)
					{
						nwrite = write(sockfd, buf.data() + data_size - n, n);
						if (nwrite < n)
						{
							if (nwrite == -1 && errno != EAGAIN)
							{
								perror("write error");
							}
							break;
						}
						n -= nwrite;
					}
					printf("send to client data [%s]\n", buf);
					close(sockfd);
					m_events[i].data.fd = -1;
				}
			}
		}
	}
};