#include "ClientNetActor.h"

#include <iostream>
#include <queue>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <unistd.h>

#include "../Server/ServerTypeDefine.h"

std::shared_ptr<IActorPimpl> IActorPimpl::m_pimpl;

ClientNetActor::ClientNetActor() : 
	m_buffer(std::make_shared<ByteBuffer>())
{

}

void ClientNetActor::SetClientType(NetServerType::ClientType client_type)
{
	m_client_type = client_type;
}

bool ClientNetActor::ReadReady()
{
	bool ret = true;

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
				ret = false;
			}
			break;
		}

		Parsing(buffer, bytes_read);
		// data_buffer.append(buffer, bytes_read);
	}

	//client triggered EPOLLIN but sent no data (usually due to remote socket being closed)

	// std::cout << "[i] client %s:%d said: %s\n", inet_ntoa(client_addr_), client_port_, data_buffer.c_str());

	//update last active time to prevent timeout
	m_last_active = time(0);

	return ret;
}

bool ClientNetActor::WriteReady()
{
	/*
		during heavy network I/O fds can become unwritable and subsequent calls to write() / send() will fail,
		in this case the data which failed to send should be stored in a buffer and the operation should be
		retried when WriteReady() is called to signal the fd is writable again (this is up to you to implement).
	*/
	std::cout << "WriteReady()" << std::endl;
	return true;
}

bool ClientNetActor::HeartBeat()
{
	// if no operations occurred during timeout period return false to signal server to close socket
	if (static_cast<time_t>(m_last_active + m_timeout) < time(0))
	{
		std::cout << "[i] connection " << inet_ntoa(m_client_sin.sin_addr) << ":" << ntohs(m_client_sin.sin_port) << " has timed out" << std::endl;
		return false;
	}

	return true;
}

void ClientNetActor::ClientClose()
{
	close(m_client_fd);

	std::cout << "[-] connection " << inet_ntoa(m_client_sin.sin_addr) << ":" << ntohs(m_client_sin.sin_port) << " closed by client" << std::endl;
}

std::shared_ptr<ByteBuffer>& ClientNetActor::GetBuffer()
{
	return m_buffer;
}

/// <summary>
/// ProccessIO
/// </summary>
void ClientNetActor::ProccessIO()
{
	switch (m_client_impl->m_config_module->GetServerType())
	{
	case NetServerType::ServerType::FRONTEND: // 主机是前端服务器
	{
		switch (m_client_type)
		{
		case NetServerType::ClientType::CLIENT: // 玩家发送的信息 直接处理
			// frontend server
			m_client_impl->m_proccess_module->ProcessFrontendIO(*this);
			break;
		case NetServerType::ClientType::BACKEND: // 后端发来的数据 直接找到客户端 返回客户端 (maybe update client data)
			// backend server
			m_client_impl->m_proccess_module->ProcessBackendIO(*this);
			break;
		case NetServerType::ClientType::MASTER:
			break;
		default:
			// (login)
			m_client_impl->m_proccess_module->ProcessFrontendUnknowMsg(*this);
			break;
		}

		// exit
		break;
	}
	case NetServerType::ServerType::BACKEND: // 主机是后端服务器
	{
		switch (m_client_type)
		{
		case NetServerType::ClientType::FRONTEND:
			m_client_impl->m_proccess_module->ProcessServerBackendIO(*this);
			break;
		case NetServerType::ClientType::BACKEND:
			// backend server
			m_client_impl->m_proccess_module->ProcessRPCIO(*this);
			break;
		case NetServerType::ClientType::MASTER:
			break;
		default:
			// (server_online)
			break;
		}
		break;
	}
	case NetServerType::ServerType::MASTER:
	{
		m_client_impl->m_proccess_module->ProcessMasterIO(*this);
		break;
	}
	default:
		break;
	}

	// reset msg
	m_buffer = std::make_shared<ByteBuffer>();
}

/// <summary>
/// Parsing
/// </summary>
/// <param name="buffer"></param>
/// <param name="len"></param>
void ClientNetActor::Parsing(std::array<char, DEFAULT_BUFLEN>& buffer, ssize_t len)
{
	auto buf_remain_len = len;
	if (m_buffer->GetHeaderStatus() == false)
	{
		// is not read length
		auto remain_len = m_buffer->GetRemainingLen();
		auto len_buf = remain_len < len ? remain_len : len;

		//
		m_buffer->RecvHeader(buffer.data(), len_buf);
		buf_remain_len -= len_buf;

		//
		if (m_buffer->GetRemainingLen() == 0)
		{
			if (m_buffer->ResetHeader() == false)
			{
				return;
			}

			if (m_buffer->GetRemainingLen() == 0)
			{
				// io actor
				ProccessIO();

				// reset msg data
				m_buffer->ResetData();
				if (buf_remain_len != 0)
				{
					std::array<char, DEFAULT_BUFLEN> buf;
					std::memcpy(buf.data(), buffer.data() + (len - buf_remain_len), buf_remain_len);
					Parsing(buf, buf_remain_len);
					return;
				}
			}
		}
	}

	if (buf_remain_len != 0)
	{
		//
		auto unreceived_len = m_buffer->GetRemainingLen();
		auto push_data_len = unreceived_len < buf_remain_len ? unreceived_len : buf_remain_len;
		m_buffer->PushData(buffer.data() + (len - buf_remain_len), push_data_len);
		buf_remain_len -= push_data_len;
	}

	if (m_buffer->GetRemainingLen() == 0)
	{
		// io
		ProccessIO();

		// reset msg data
		m_buffer->ResetData();
		if (buf_remain_len != 0)
		{
			std::array<char, DEFAULT_BUFLEN> buf;
			std::memcpy(buf.data(), buffer.data() + (len - buf_remain_len), buf_remain_len);
			Parsing(buf, buf_remain_len);
		}
	}
}

/// <summary>
/// SendData
/// </summary>
/// <param name="value"></param>
void ClientNetActor::SendData(const int major, const int minor, const char* ptr_value, const int length)
{
	std::vector<char> temp_data(HEADER_LENGTH + length);
	std::memcpy(temp_data.data(), &major, sizeof(MAJOR_LENGTH)); // copy major
	std::memcpy(temp_data.data() + MAJOR_LENGTH, &minor, sizeof(MINOR_LENGTH)); // copy minor
	std::memcpy(temp_data.data() + MAJOR_LENGTH + MINOR_LENGTH, &length, sizeof(INT_LENGTH)); // copy length
	if (length != 0)
	{
		std::memcpy(temp_data.data() + HEADER_LENGTH, ptr_value, length);
	}
	send(m_client_fd, temp_data.data(), HEADER_LENGTH + length, 0);
}

void ClientNetActor::SendBuffer(std::shared_ptr<ByteBuffer> buffer)
{
	int buffer_size = buffer->GetSize();

	// build buffer
	std::vector<char> temp_data(HEADER_LENGTH + buffer_size);
	std::memcpy(temp_data.data(), buffer->GetHeader(), HEADER_LENGTH);
	std::memcpy(temp_data.data() + HEADER_LENGTH, buffer->GetBuffer(), buffer_size);

	//
	send(m_client_fd, temp_data.data(), HEADER_LENGTH + buffer_size, 0);
}

void ClientNetActor::SendStream(std::vector<char> stream)
{
	send(m_client_fd, stream.data(), stream.size(), 0);
}