#include "PollModule.h"

#include <iostream>
#include <algorithm>
#include <cstring>

#include "PollMasterClient.h"
#include "../Server/ByteBuffer.h"
#include "../Server/StringDefine.h"
#include "../Server/PackageNetMsg.h"
#include "../Server/BuiltInMsgDefine.h"

void PollModule::Init()
{
	// module
	m_client_net_module = m_ptr_manager->GetModule<IClientNetModule>();
	m_server_obj_module = m_ptr_manager->GetModule<IServerObjModule>();
	m_config_module = m_ptr_manager->GetModule<IConfigModule>();

	// init fd
	InitFD();
}

void PollModule::AfterInit()
{
	if (m_config_module->GetServerType() != EnumDefine::ServerType::MASTER)
	{
		std::cout << CONNECT_TO_MASTER_SERVER << std::endl;
		ConnectMasterServer();
	}
}

void PollModule::InitFD()
{
	std::for_each(std::begin(m_arr_poll_fd), std::end(m_arr_poll_fd), [](struct pollfd& item) -> void
		{
			item.fd = -1;
		}
	);
}

bool PollModule::ConnectMasterServer()
{
	// ret
	bool ret = true;

	auto vec_server_name = m_config_module->GetServersByType(STR_MASTER);
	auto server_data = m_config_module->GetServerDataByName(vec_server_name[0]->m_server_name);

	//
	while (true)
	{
		// create master client
		auto ptr_client{ m_client_net_module->CreatePollMasterClient() };

		// connect master
		if (false == ConnectServer(server_data->m_server_ip, server_data->m_port, ptr_client->m_fd))
		{
			ret = false;
			std::perror(CAN_NOT_CONNECT_MASTER);
			break;
		}

		// add map
		AddClientToMap(vec_server_name[0]->m_server_name, ptr_client);

		// send this server is online
		ConnectServerOnline connect_server_online;
		connect_server_online.m_server_name = m_config_module->GetMyServerInfo()->m_server_name;

		// package
		std::vector<char> msg_buffer;
		PackageStructForEachField(connect_server_online, msg_buffer);
		ptr_client->SendData(static_cast<int>(BuiltInMsg::ServerMsg::SERVER_ONLINE), 0, msg_buffer);

		// exit while
		break;
	}

	return ret;
}

bool PollModule::ConnectServer(const std::string& ip, const int port, int& fd)
{
	// return
	bool ret = true;

	// socket
	fd = socket(AF_INET, SOCK_STREAM, 0);

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
	}

	return ret;
}

void PollModule::AddSocket(int fd)
{
	for (auto item : m_arr_poll_fd)
	{
		if (item.fd < 0)
		{
			item.fd = fd;
			item.events = POLLIN | POLLRDHUP | POLLERR;
			++m_nfds;

			// exit
			break;
		}
	}
}

void PollModule::AddClientToMapByFD(const int fd, std::shared_ptr<IPollClient> ptr_client)
{
	m_map_client_fd.emplace(fd, ptr_client);
}

std::shared_ptr<IPollClient> PollModule::GetClientFromMapByFD(const int fd)
{
	std::shared_ptr<IPollClient> ptr_client = nullptr;

	if (m_map_client_fd.find(fd) != std::cend(m_map_client_fd))
	{
		ptr_client = m_map_client_fd[fd];
	}

	return ptr_client;
}

/// <summary>
/// 
/// </summary>
/// <param name="server_name"></param>
/// <param name="ptr_client"></param>

void PollModule::AddClientToMap(const std::string& server_name, std::shared_ptr<IPollClient> ptr_client)
{
	m_map_client_net.emplace(server_name, ptr_client);
}

bool PollModule::HandleRead(struct pollfd& poll_fd)
{
	std::array<char, DEFAULT_BUFLEN> buf;
	ssize_t s = read(poll_fd.fd, buf.data(), DEFAULT_BUFLEN);
	if (s > 0)
	{
		auto ptr_client = GetClientFromMapByFD(poll_fd.fd);
		if (ptr_client != nullptr)
		{
			ptr_client->PushData(buf.data(), s);
		}

		// fflush(stdout);
		// poll_fd.events = POLLOUT;
	}
	else
	{
		close(poll_fd.fd);
		poll_fd.fd = -1;
	}

	return true;
}

// interface

void PollModule::EventLoop()
{
	auto ret = 0;
	auto timeout = -1;

	while (true)
	{
		switch (ret = poll(m_arr_poll_fd.data(), m_nfds, timeout))
		{
		case 0:
			std::cerr << "timeout..." << std::endl;
			break;
		case -1:
			std::cerr << "poll error" << std::endl;
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

bool PollModule::ConnectServerWithServerName(const std::string& ip, const int port, const std::string& server_name)
{
	// log
	std::cout << CONNECT_SERVER << server_name << std::endl;

	bool ret = true;

	//
	auto vec_server_name = m_config_module->GetServersByType(STR_MASTER);
	auto server_data = m_config_module->GetServerDataByName(vec_server_name[0]->m_server_name);

	// create master client
	auto ptr_client{ m_client_net_module->CreatePollClient() };

	// connect master
	if (false == ConnectServer(server_data->m_server_ip, server_data->m_port, ptr_client->m_fd))
	{
		ret = false;
		std::perror(CAN_NOT_CONNECT_OTHER_SERVER);
	}
	else
	{
		// add map
		AddClientToMap(vec_server_name[0]->m_server_name, ptr_client);
	}

	return ret;
}

std::shared_ptr<IPollClient> PollModule::GetClientByServerName(const std::string& server_name)
{
	std::shared_ptr<IPollClient> ptr_client{ nullptr };
	if (m_map_client_net.find(server_name) != std::cend(m_map_client_net))
	{
		ptr_client = m_map_client_net[server_name];
	}
	return ptr_client;
}