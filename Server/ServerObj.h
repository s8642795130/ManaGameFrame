#pragma once
#include "ClientNet.h"

class ServerObj : public ClientNet
{
public:
	ServerObj() = default;
	virtual ~ServerObj() = default;

	bool ConnectServer(const std::string& ip, const short unsigned int port)
	{
        bool ret = true;

        m_client_fd = socket(AF_INET, SOCK_STREAM, 0);

        // test code
        int on = 1;
        setsockopt(m_client_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

        // sockaddr_in
        std::memset(&m_client_sin, 0, sizeof(m_client_sin));
        m_client_sin.sin_family = AF_INET;
        m_client_sin.sin_port = htons(port); //
        m_client_sin.sin_addr.s_addr = inet_addr(ip.c_str()); //

        //
        if (connect(m_client_fd, (struct sockaddr*)&m_client_sin, sizeof(m_client_sin)) < 0)
        {
            ret = false;
        }

        return ret;
	}

    bool RecvData()
    {
        return true;
    }
};
