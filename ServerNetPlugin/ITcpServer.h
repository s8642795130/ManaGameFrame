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

#include <array>
#include <memory>

#include "INetActor.h"
#include "../Server/DefineHeader.h"
#include "../Server/IPluginManager.h"
#include "IClientNetModule.h"

class ITcpServer
{
private:
	// manager
	std::shared_ptr<IPluginManager> m_ptr_manager;
	// module
	std::shared_ptr<IThreadPoolModule> m_thread_pool_module;
	std::shared_ptr<IClientNetModule> m_client_net_module;

	//
	const int m_worker_max_events = WORKER_MAX_EVENTS;
	int m_listen_fd = -1;
	int m_epoll_fd = -1;
	std::array<epoll_event, WORKER_MAX_EVENTS> m_arr_events;
public:
	ITcpServer(std::shared_ptr<IPluginManager> ptr) : m_ptr_manager(ptr)
	{
		m_thread_pool_module = m_ptr_manager->GetModule<IThreadPoolModule>();
		m_client_net_module = m_ptr_manager->GetModule<IClientNetModule>();
	}

	void StartNetwork(uint16_t listen_port, uint32_t timeout_secs)
	{
		// test code
		// timeout_secs_ = timeout_secs;

		//
		sockaddr_in sin = { 0 };

		// sin.sin_addr.s_addr = inet_addr(listen_addr);
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
		sin.sin_family = AF_INET;
		sin.sin_port = htons(listen_port);

		m_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (m_listen_fd <= 0)
		{
			throw std::runtime_error("socket() failed, error code: " + std::to_string(errno));
		}

		if (SetNonblocking(m_listen_fd) == false)
		{
			throw std::runtime_error("SetNonBlocking() failed, error code: " + std::to_string(errno));
		}

		if (bind(m_listen_fd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin)))
		{
			throw std::runtime_error("bind() failed, error code: " + std::to_string(errno));
		}

		if (listen(m_listen_fd, SOMAXCONN) == -1)
		{
			throw std::runtime_error("listen() failed, error code: " + std::to_string(errno));
		}

		CreateEpoll();

		epoll_event e_event;
		e_event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
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

	bool AddFD(std::shared_ptr<INetActor> ptr_client)
	{
		if (!SetNonblocking(ptr_client->m_client_fd))
		{
			throw std::runtime_error("failed to put fd into non-blocking mode, error code: " + std::to_string(errno));
			return false;
		}

		epoll_event ev;
		// ev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;	// client events will be handled in edge-triggered mode
		ev.data.fd = ptr_client->m_client_fd;
		// ev.data.ptr = reinterpret_cast<void*>(ptr_client.get()); // we will pass client descriptor with every event
		ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;

		if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, ptr_client->m_client_fd, &ev) == -1)
		{
			throw std::runtime_error("epoll_ctl() failed, error code: " + std::to_string(errno));
			return false;
		}

		// store new client descriptor into the map of clients
		m_client_net_module->AddClientToMap(ptr_client);
		m_thread_pool_module->AddActorToThreadCell(ptr_client);

		return true;
	}

	void EventLoop()
	{
		while (true)
		{
			int num_fds = epoll_wait(m_epoll_fd, m_arr_events.data(), m_worker_max_events, -1);
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

	bool SetNonblocking(int fd)
	{
		int flags = fcntl(fd, F_GETFL);
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
		std::shared_ptr<INetActor> ptr_client = CreateClientNet();

		// get sin struct size
		socklen_t sin_size = sizeof(ptr_client->m_client_sin);

		ptr_client->m_client_fd = accept(m_listen_fd, reinterpret_cast<sockaddr*>(&ptr_client->m_client_sin), &sin_size);
		if (ptr_client->m_client_fd == -1)
		{
			if (errno != EAGAIN && errno != ECONNABORTED && errno != EPROTO && errno != EINTR)
			{
				std::perror("accept");
			}
			std::cout << "accept() failed, error code: " << errno << std::endl;
			return false;
		}

		return AddFD(ptr_client);
	}

	//called whenever and EPOLLIN event occurs on a client fd
	bool HandleClient(epoll_event& ev)
	{
		// retrieve client descriptor address from the data parameter (test code)
		// IClientNetActor* client = reinterpret_cast<IClientNetActor*>(ev.data.ptr);
		//
		std::shared_ptr<INetActor> ptr_client = m_client_net_module->GetClientNet(ev.data.fd);

		//we got some data from the client
		if (ev.events & EPOLLIN)
		{
			if (!ptr_client->OnReadReady())
			{
				ptr_client->OnServerClose();
				RemoveClient(ptr_client);
				// delete client;

				return false;
			}
		}

		//the client closed the connection (should be after EPOLLIN as client can send data then close)
		if (ev.events & EPOLLRDHUP)
		{
			ptr_client->OnServerClose();
			RemoveClient(ptr_client);
			// delete client;

			return false;
		}

		//fd is ready to be written
		if (ev.events & EPOLLOUT)
		{
			if (!ptr_client->OnWriteReady())
			{
				ptr_client->OnServerClose();
				RemoveClient(ptr_client);
				// delete client;
				return false;
			}
		}

		return true;
	}

	void RemoveClient(std::shared_ptr<INetActor>& ptr_client)
	{
		m_client_net_module->RemoveClientFromMap(ptr_client->GetSid());
		m_client_net_module->RemoveLoginClientFromMap(ptr_client->GetUid());
		m_thread_pool_module->RemoveActorFromThreadCell(ptr_client->GetUUID());
	}

	virtual std::shared_ptr<INetActor> CreateClientNet() = 0;
};