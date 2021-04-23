#pragma once
#include "ClientNet.h"

class ServerObj : public ClientNet
{
public:
	ServerObj() = default;
	virtual ~ServerObj() = default;

	void ConnectServer(const std::string& ip, const short unsigned int port)
	{
        int sock = socket(AF_INET, SOCK_STREAM, 0);

        // test code
        int on = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

        // sockaddr_in
        std::memset(&m_client_sin, 0, sizeof(m_client_sin));
        m_client_sin.sin_family = AF_INET;
        m_client_sin.sin_port = htons(port); //
        m_client_sin.sin_addr.s_addr = inet_addr(ip.c_str()); //

        //
        if (connect(sock, (struct sockaddr*)&m_client_sin, sizeof(m_client_sin)) < 0)
        {
            std::cout << "connect" << std::endl;
        }
	}
};
