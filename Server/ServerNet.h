#pragma once

#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <vector>
#include <errno.h>
#include <stdexcept>
#include <unistd.h>
#include <time.h>

#include <string>
#include <map>
#include <functional>
#include <iostream>

#include "DefineHeader.h"
#include "ClientDescriptor.h"
#include "ClientNet.h"

// template <class ClientDescriptorType> class ServerNet
class ServerNet
{
private:
	const int m_worker_max_events = WORKER_MAX_EVENTS;
	int m_listen_fd = -1;
	int m_epoll_fd = -1;
	std::array<epoll_event, WORKER_MAX_EVENTS> m_arr_events;
	std::map<int, ClientDescriptor*> m_map_clients;
	uint32_t timeout_secs_;
	time_t last_socket_check_;

	//
	std::shared_ptr<std::map<int, std::function<void(ClientDescriptor*)>>> m_ptr_callback_map;

public:
	ServerNet() :
		m_listen_fd(-1),
		m_epoll_fd(-1),
		last_socket_check_(0)
	{
		// test code
		m_ptr_callback_map = std::make_shared<std::map<int, std::function<void(ClientDescriptor*)>>>();
	}

	~ServerNet()
	{
		if (m_listen_fd != -1)
		{
			close(m_listen_fd);
		}
			
		if (m_epoll_fd != -1)
		{
			close(m_epoll_fd);
		}
	}

	void StartNetwork(const char* listen_addr, uint16_t listen_port, uint32_t timeout_secs)
	{
		// test code
		timeout_secs_ = timeout_secs;

		//
		sockaddr_in sin = { 0 };

		sin.sin_addr.s_addr = inet_addr(listen_addr);
		sin.sin_family = AF_INET;
		sin.sin_port = htons(listen_port);

		m_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (m_listen_fd <= 0)
		{
			throw std::runtime_error("socket() failed, error code: " + std::to_string(errno));
		}

		if (bind(m_listen_fd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin)))
		{
			throw std::runtime_error("bind() failed, error code: " + std::to_string(errno));
		}

		if (SetNonblocking(m_listen_fd) == false)
		{
			throw std::runtime_error("SetNonBlocking() failed, error code: " + std::to_string(errno));
		}

		if (listen(m_listen_fd, SOMAXCONN) == -1)
		{
			throw std::runtime_error("listen() failed, error code: " + std::to_string(errno));
		}

		CreateEpoll();

		epoll_event e_event;
		e_event.events = EPOLLIN;
		e_event.data.fd = m_listen_fd;

		if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_listen_fd, &e_event) == -1)
		{
			throw std::runtime_error("epoll_ctl() failed, error code: " + std::to_string(errno));
		}
	}

	void CreateEpoll()
	{
		m_epoll_fd = epoll_create1(0);
		if (m_epoll_fd == -1)
		{
			throw std::runtime_error("epoll_create1() failed, error code: " + std::to_string(errno));
		}
	}

	bool AddFD(ClientDescriptor* ptr_client)
	{
		if (!SetNonblocking(ptr_client->m_client_fd))
		{
			std::cout << "failed to put fd into non-blocking mode, error code: " << errno << std::endl;
			return false;
		}

		epoll_event ev;
		ev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;	// client events will be handled in edge-triggered mode
		ev.data.ptr = ptr_client;						// we will pass client descriptor with every event

		if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, ptr_client->m_client_fd, &ev) == 1)
		{
			std::cout << "epoll_ctl() failed, error code: " << errno << std::endl;
			// delete ptr_client;
			return false;
		}

		// store new client descriptor into the map of clients
		m_map_clients[ptr_client->m_client_fd] = ptr_client;

		// set callback map ptr
		ptr_client->SetReceiveCallBackMapPtr(m_ptr_callback_map);

		return true;
	}

	void AddReceiveCallBack(const int msgID, std::function<void(ClientDescriptor*)> call_func)
	{
		// test code
		m_ptr_callback_map->emplace(msgID, call_func);
	}

	void EventLoop()
	{
		while (true)
		{
			int num_fds = epoll_wait(m_epoll_fd, m_arr_events.data(), m_worker_max_events, 1000);
			if (num_fds != -1)
			{
				//iterate signaled fds
				for (int i = 0; i < num_fds; i++)
				{
					//notifications on listening fd are incoming client connections
					if (m_arr_events[i].data.fd == m_listen_fd)
					{
						HandleAccept();
					}
					else
					{
						HandleClient(m_arr_events[i]);
					}
				}
			}

			//perform cleanup every second and remove timed-out sockets
			/*
			if ((last_socket_check_ + 1) < time(0) && m_map_clients.size() > 0)
			{
				std::map<int, ClientDescriptor*>::iterator it = m_map_clients.begin();
				while (it != m_map_clients.end())
				{
					ClientDescriptor* client = (*it).second;

					if (!client->HeartBeat())
					{
						//if HeartBeat() returns false remove fd from map and close
						it = clients_.erase(it);
						client->ServerClose();
						delete client;
					}
					else {
						it++;
					}
				}

				last_socket_check_ = time(0);
			}
			*/
		}
	}

private:
	bool SetNonblocking(int fd)
	{
		int flags = fcntl(fd, F_GETFL, 0);
		if (flags == -1)
		{
			return false;
		}
			
		flags |= O_NONBLOCK;

		if (fcntl(fd, F_SETFL, flags) == -1)
		{
			return false;
		}

		return true;
	}

	//called whenever an EPOLLIN event occurs on the server fd
	bool HandleAccept()
	{
		// allocate and initialize a new descriptor for the client
		ClientNet* ptr_client = new ClientNet();

		// get sin struct size
		socklen_t sin_size = sizeof(ptr_client->m_client_sin);

		ptr_client->m_client_fd = accept(m_listen_fd, reinterpret_cast<sockaddr*>(&ptr_client->m_client_sin), &sin_size);
		if (ptr_client->m_client_fd == -1)
		{
			std::cout << "accept() failed, error code: " << errno << std::endl;
			return false;
		}

		return AddFD(ptr_client);
		/*
		if (!SetNonblocking(client->m_client_fd))
		{
			std::cout << "failed to put fd into non-blocking mode, error code: " << errno << std::endl;
			return false;
		}

		epoll_event ev;
		ev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;	//client events will be handled in edge-triggered mode
		ev.data.ptr = client;						//we will pass client descriptor with every event

		if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, client->m_client_fd, &ev) == 1)
		{
			std::cout << "epoll_ctl() failed, error code: " << errno << std::endl;
			delete client;
			return false;
		}

		//store new client descriptor into the map of clients
		m_map_clients[client->m_client_fd] = client;
		*/

		// std::cout << "[+] new client: " << inet_ntoa(client_sin.sin_addr) << ":" << ntohs(client_sin.sin_port) << std::endl;
	}

	//called whenever and EPOLLIN event occurs on a client fd
	bool HandleClient(epoll_event ev)
	{
		//retrieve client descriptor address from the data parameter
		ClientDescriptor* client = reinterpret_cast<ClientDescriptor*>(ev.data.ptr);

		//we got some data from the client
		if (ev.events & EPOLLIN)
		{
			if (!client->ReadReady())
			{
				client->ServerClose();
				RemoveClient(client);
				delete client;

				return false;
			}
		}

		//the client closed the connection (should be after EPOLLIN as client can send data then close)
		if (ev.events & EPOLLRDHUP)
		{
			client->ServerClose();
			RemoveClient(client);
			 delete client;

			return false;
		}

		//fd is ready to be written
		if (ev.events & EPOLLOUT)
		{
			if (!client->WriteReady())
			{
				client->ServerClose();
				RemoveClient(client);
				// delete client;
				return false;
			}
		}

		return true;
	}

	void RemoveClient(ClientDescriptor* ptr_client)
	{
		std::map<int, ClientDescriptor*>::iterator it = m_map_clients.find(ptr_client->GetSid());
		m_map_clients.erase(it);
	}
};