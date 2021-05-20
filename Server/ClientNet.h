#pragma once

#include <iostream>
#include <queue>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <unistd.h>

#include "ClientDescriptor.h"
#include "GameMessage.h"
#include "IConfigFile.h"
#include "ServerType.h"

class ClientNet : public ClientDescriptor
{
protected:
	time_t m_last_active;
	//
	bool m_is_login = false;
	std::queue<std::unique_ptr<ByteBuffer>> m_queue_msg;
public:
	ClientNet()
	{

	}

	void SetClientType(NetMessage::ClientType client_type)
	{
		m_client_type = client_type;
	}

	virtual bool ReadReady()
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

	virtual bool WriteReady()
	{
		/*
			during heavy network I/O fds can become unwritable and subsequent calls to write() / send() will fail,
			in this case the data which failed to send should be stored in a buffer and the operation should be
			retried when WriteReady() is called to signal the fd is writable again (this is up to you to implement).
		*/
		std::cout << "WriteReady()" << std::endl;
		return true;
	}

	virtual bool HeartBeat()
	{
		// if no operations occurred during timeout period return false to signal server to close socket
		if (static_cast<time_t>(m_last_active + m_timeout) < time(0))
		{
			std::cout << "[i] connection " << inet_ntoa(m_client_sin.sin_addr) << ":" << ntohs(m_client_sin.sin_port) << " has timed out" << std::endl;
			return false;
		}

		return true;
	}

	virtual void ClientClose()
	{
		close(m_client_fd);

		std::cout << "[-] connection " << inet_ntoa(m_client_sin.sin_addr) << ":" << ntohs(m_client_sin.sin_port) << " closed by client" << std::endl;
	}

	virtual void ServerClose()
	{
		close(m_client_fd);

		std::cout << "[-] connection " << inet_ntoa(m_client_sin.sin_addr) << ":" << ntohs(m_client_sin.sin_port) << " closed by server" << std::endl;
	}

	virtual void SetReceiveCallbackMapPtr(std::shared_ptr<std::map<int, std::function<void(ClientDescriptor*)>>> receive_callBack)
	{
		m_receive_callBack = receive_callBack;
	}

	// Client 向前端服务器发来的消息 (根据路由找到对应的后端发送)
	void ProcessFrontendIO()
	{
		// send to backend
		if (!m_is_login)
		{
			return;
		}

		// router
		int majorId = m_buffer->GetMajorId();
		auto map_msg = NetMsgDefine::GetNetMsg();
		auto plugin_name = map_msg[majorId];
		auto config_file = this->m_app->GetConfigPtr();
		const auto server_list = config_file->GetServersByPluginName(plugin_name);
	}

	void ProcessBackendIO()
	{
		// (rpc) (msg) (rcp callback)
		/*
		auto thread_index = ThreadRouter::GetThreadActorIndexBySocket(m_client_fd);
		std::unique_ptr<IActorMsg> ptr = std::make_unique<ActorMsg<void, ClientNetControlActor, int, int>>("", actor_uuid, &ClientNetControlActor::ProcClientMessage, 10, 20);
		m_app->SendMsgToActor(ptr);
		*/

		const int frontend_msg = static_cast<std::underlying_type_t<NetMessage::ServerMsg>>(NetMessage::ServerMsg::FRONTEND_MSG);
		std::function<void(ClientDescriptor*)> callback = (*m_receive_callBack)[frontend_msg];
		callback(this);
	}

	void ProcessFrontendUnknowMsg()
	{

	}

	void ProcessServerBackendIO()
	{

	}

	void ProcessRPCIO()
	{

	}

	void ProcessMasterIO()
	{
		int majorId = m_buffer->GetMajorId();
		std::function<void(ClientDescriptor*)> callback = (*m_receive_callBack)[majorId];
		callback(this);
	}

	/// <summary>
	/// ProccessIO
	/// </summary>
	void ProccessIO()
	{
		switch (ServerType::GetServerType())
		{
		case NetMessage::ServerType::FRONTEND: // 主机是前端服务器
		{
			switch (m_client_type)
			{
			case NetMessage::ClientType::CLIENT: // 玩家发送的信息 直接处理
				// frontend server
				ProcessFrontendIO();
				break;
			case NetMessage::ClientType::BACKEND: // 后端发来的数据 直接找到客户端 返回客户端
				// backend server
				ProcessBackendIO();
				break;
			case NetMessage::ClientType::MASTER:
				break;
			default:
				// (login)
				ProcessFrontendUnknowMsg();
				break;
			}
			
			// exit
			break;
		}
		case NetMessage::ServerType::BACKEND: // 主机是后端服务器
		{
			switch (m_client_type)
			{
			case NetMessage::ClientType::FRONTEND:
				ProcessServerBackendIO();
				break;
			case NetMessage::ClientType::BACKEND:
				// backend server
				ProcessRPCIO();
				break;
			case NetMessage::ClientType::MASTER:
				break;
			default:
				// (server_online)
				break;
			}
			break;
		}
		case NetMessage::ServerType::MASTER:
		{
			ProcessMasterIO();
			break;
		}
		default:
			break;
		}
	}

	/// <summary>
	/// Parsing
	/// </summary>
	/// <param name="buffer"></param>
	/// <param name="len"></param>
	void Parsing(std::array<char, DEFAULT_BUFLEN>& buffer, ssize_t len)
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
	virtual void SendData(const int major, const int minor, const char* ptr_value, const int length)
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
};