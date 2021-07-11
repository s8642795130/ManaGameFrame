#include "PollModule.h"

#include <iostream>
#include <algorithm>
#include <cstring>

void PollModule::Init()
{
	// module
	m_client_net_module = m_ptr_manager->GetModule<IClientNetModule>();
	m_server_obj_module = m_ptr_manager->GetModule<IServerObjModule>();

	// init fd
	InitFD();
}

void PollModule::AfterInit()
{
	ConnectMasterServer();
}

void PollModule::InitFD()
{
	std::for_each(std::begin(m_arr_poll_fd), std::end(m_arr_poll_fd), [](struct pollfd& item) -> void
		{
			item.fd = -1;
		}
	);
}

bool PollModule::ConnectMasterServer(const std::string& ip, int port)
{
	if (m_master_client_actor.ConnectMaster(ip, port))
	{
		AddSocket(m_master_client_actor->m_fd);
	}
	else
	{
		std::cerr << "const master err" << std::endl;
	}
}

bool PollModule::ConnectServer(const std::string& ip, const int port, const std::string& server_name)
{
	// return
	bool ret = true;

	// socket
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	// test code
	int on = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	// sockaddr_in
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(static_cast<uint16_t>(port));
	local.sin_addr.s_addr = inet_addr(ip.c_str());

	if (connect(fd, (struct sockaddr*)&local, sizeof(local)) < 0)
	{
		fprintf(stderr, "socket connect error = %d ( %s )\n", errno, std::strerror(errno));
		ret = false;
	}
	else
	{
		AddSocket(fd);
		AddActorToMap(fd, server_name);
	}

	return ret;
}

void PollModule::AddSocket(int fd)
{
	for (int j = 1; j < m_max_poll_count; j++)
	{
		if (m_arr_poll_fd[j].fd < 0)
		{
			m_arr_poll_fd[j].fd = fd;
			m_arr_poll_fd[j].events = POLLIN | POLLRDHUP | POLLERR;
			if (j + 1 > m_nfds)
			{
				m_nfds = j + 1;
			}
			break;
		}
	}
}

void PollModule::AddActorToMap(int fd, const std::string& server_name)
{
	std::shared_ptr<BackendPollActor> backend_actor{ std::make_shared<BackendPollActor>() };
	backend_actor->m_conn_id = fd;

	// add
	m_server_obj_module->SaveServerToMap(server_name, backend_actor->GetUUID());
}

void PollModule::StartPoll()
{
	auto ret = 0;
	auto timeout = -1;

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
			auto temp_count = 0;
			while (temp_count == ret)
			{
				if (m_arr_poll_fd[temp_count].fd != -1 && m_arr_poll_fd[temp_count].revents & POLLIN)
				{
					HandleRead(m_arr_poll_fd[temp_count]);
				}
				++temp_count;
			}
		}
		break;
		}
	}
}

bool PollModule::HandleRead(struct pollfd& poll_fd)
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

// interface

void PollModule::SendBufferByServerName(std::vector<char> buffer, const std::string& server_name)
{
	m_map_client_net[server_name]->SendData();
}