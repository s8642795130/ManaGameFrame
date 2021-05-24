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
#include "ServerController.h"
#include "MsgRouter.h"
#include "CommonStruct.h"
#include "GameMessageData.h"
#include "MessageData.h"

class ClientNet : public ClientDescriptor
{
protected:
	time_t m_last_active;
	//
	bool m_is_login = false;
	bool m_is_working = false;
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

		// get msg corresponding to plugin
		const auto majorId = m_buffer->GetMajorId();
		auto map_msg = NetMsgDefine::GetNetMsg();
		auto plugin_name = map_msg[majorId];

		// get all the servers that the plugin exists
		auto config_file = m_app->GetConfigPtr();
		const auto server_list = config_file->GetServersByPluginName(plugin_name);

		// router
		const auto server_index = MsgRouter::GetMsgRouterByClient(plugin_name, static_cast<int>(server_list.size()), *this);

		// get server uuid
		auto server_map = ServerController::GetServerMap();
		auto server_uuid = server_map[server_list[server_index]->m_server_name];

		// send to backend server
		std::unique_ptr<IActorMsg> ptr = std::make_unique<ActorMsg<void, ClientNet, std::shared_ptr<ByteBuffer>>>(GetUUID(), server_uuid, &ClientNet::SendBuffer, std::move(m_buffer));
		m_app->SendMsgToActor(ptr);

		//
		m_buffer = std::make_shared<ByteBuffer>();
	}

	void ProcessBackendIO()
	{
		// there is two situations, case 1: back msg to client; case 2: change client data
		if (m_buffer->GetMajorId() != static_cast<int>(NetMessage::ServerMsg::RETURN_CLIENT_MSG))
		{
			// unpack msg
			BackendMsgToClient backend_msg;
			ForEachField(backend_msg, m_buffer);

			// send to client
			std::unique_ptr<IActorMsg> ptr = std::make_unique<ActorMsg<void, ClientNet, std::shared_ptr<ByteBuffer>>>(GetUUID(), backend_msg.m_client_uuid, &ClientNet::SendBuffer, std::move(backend_msg.m_buffer));
			m_app->SendMsgToActor(ptr);
		}
		else if (m_buffer->GetMajorId() != static_cast<int>(NetMessage::ServerMsg::UPDATE_CLIENT_MSG))
		{

		}
		else
		{
			// error...
		}
	}

	void ProcessFrontendUnknowMsg()
	{
		// check login info msg
		if (m_buffer->GetMajorId() != static_cast<int>(NetMessage::ServerMsg::LOGIN_MSG)) // LOGIN_MSG
		{
			return;
		}
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
			case NetMessage::ClientType::BACKEND: // 后端发来的数据 直接找到客户端 返回客户端 (maybe update client data)
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

	virtual void SendBuffer(std::shared_ptr<ByteBuffer> buffer)
	{
		int buffer_size = buffer->GetSize();

		// build buffer
		std::vector<char> temp_data(HEADER_LENGTH + buffer_size);
		std::memcpy(temp_data.data(), buffer->GetHeader(), HEADER_LENGTH);
		std::memcpy(temp_data.data() + HEADER_LENGTH, buffer->GetBuffer(), buffer_size);

		//
		send(m_client_fd, temp_data.data(), HEADER_LENGTH + buffer_size, 0);
	}
};