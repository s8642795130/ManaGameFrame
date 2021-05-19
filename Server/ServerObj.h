#pragma once
#include "ClientNet.h"

class ServerObj : public ClientNet
{
public:
	ServerObj() = default;
	virtual ~ServerObj() = default;

	bool ConnectServer(const std::string& ip, int port)
	{
        bool ret = true;

        m_client_fd = socket(AF_INET, SOCK_STREAM, 0);

        // test code
        int on = 1;
        setsockopt(m_client_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

        // sockaddr_in
        std::memset(&m_client_sin, 0, sizeof(m_client_sin));
        m_client_sin.sin_family = AF_INET;
        m_client_sin.sin_port = htons(static_cast<uint16_t>(port)); //
        m_client_sin.sin_addr.s_addr = inet_addr(ip.c_str()); //

        //
		std::cout << "connect xxx" << std::endl;
        if (connect(m_client_fd, (struct sockaddr*)&m_client_sin, sizeof(m_client_sin)) < 0)
        {
			std::cout << "connect xxxxx122" << std::endl;
            ret = false;
        }

        return ret;
	}

	bool OnceParsing(std::array<char, DEFAULT_BUFLEN>& buffer, ssize_t len, std::shared_ptr<ByteBuffer>& data_buffer)
	{
		auto buf_remain_len = len;
		if (m_buffer->GetHeaderStatus() == false)
		{
			// is not read length
			auto remain_len = m_buffer->GetRemainLen();
			auto len_buf = remain_len < len ? remain_len : len;

			//
			m_buffer->RecvHeader(buffer.data(), len_buf);
			buf_remain_len -= len_buf;

			//
			if (m_buffer->GetRemainLen() == 0)
			{
				if (m_buffer->ResetHeader() == false)
				{
					return false;
				}

				if (m_buffer->GetRemainLen() == 0)
				{
					// io actor
					// ProccessIO();
					data_buffer = std::move(m_buffer);
					m_buffer = std::make_unique<ByteBuffer>();
					return true;
				}
			}
		}

		if (buf_remain_len != 0)
		{
			//
			auto unreceived_len = m_buffer->GetUnreceivedLen();
			auto push_data_len = unreceived_len < buf_remain_len ? unreceived_len : buf_remain_len;
			m_buffer->PushData(buffer.data() + (len - buf_remain_len), push_data_len);
			buf_remain_len -= push_data_len;
		}

		if (m_buffer->GetRemainLen() == 0)
		{
			// io
			// ProccessIO();
			data_buffer = std::move(m_buffer);
			m_buffer = std::make_unique<ByteBuffer>();

			return true;
		}

		return false;
	}

    std::shared_ptr<ByteBuffer> RecvData()
    {
		std::shared_ptr<ByteBuffer> data_buffer{ std::make_shared<ByteBuffer>() };

		// recv data
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
				}
				break;
			}

			if (OnceParsing(buffer, bytes_read, data_buffer))
			{
				break;
			}
		}

		return data_buffer;
    }
};
