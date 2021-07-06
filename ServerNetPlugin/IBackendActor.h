#pragma once
#include "NetActor.h"

class IBackendActor : public NetActor
{
public:
	IBackendActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr_impl, ITcpServer* ptr_sender) :
		NetActor(ptr_manager, ptr_impl, ptr_sender)
	{
	}

	// interface
	bool ConnectServer(const std::string& ip, const int port)
	{
		bool ret = true;
		/*
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
		*/

		return ret;
	}
};