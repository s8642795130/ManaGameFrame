#pragma once
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <unistd.h>

#include "../ActorPlugin/Actor.h"
#include "../Server/ByteBuffer.h"
#include "ClientPimpl.h"
#include "HPSocket.h"

class INetActor : public Actor
{
protected:
	ITcpServer* m_ptr_sender = nullptr;
	// impl
	std::shared_ptr<ClientPimpl> m_client_impl;
	//
	std::shared_ptr<ByteBuffer> m_buffer;
public:

	unsigned long m_conn_id;

	INetActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr_impl, ITcpServer* ptr_sender) :
		Actor(ptr_manager),
		m_ptr_sender(ptr_sender),
		m_client_impl(ptr_impl),
		m_buffer(std::make_shared<ByteBuffer>())
	{
	}

	//client's unique id
	const unsigned long GetSid() const
	{
		return m_conn_id;
	}
	
	void ClientClose()
	{
		m_ptr_sender->Disconnect(m_conn_id);

		std::cout << "[-] connection " << std::endl;
	}

	virtual bool PushData(const BYTE* ptr_data, int length)
	{
		// auto buf_remain_len = length;

		/*
		if (m_buffer->GetHeaderStatus() == false)
		{
			// is not read length
			auto remain_len = m_buffer->GetRemainingLen();
			auto len_buf = remain_len < length ? remain_len : length;

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
					ProcessIO();

					// reset msg data
					m_buffer->ResetData();
					if (buf_remain_len != 0)
					{
						std::array<char, DEFAULT_BUFLEN> buf;
						std::memcpy(buf.data(), buffer.data() + (len - buf_remain_len), buf_remain_len);
						PushData(buf, buf_remain_len);
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
			m_buffer->PushData(buffer.data() + (length - buf_remain_len), push_data_len);
			buf_remain_len -= push_data_len;
		}

		if (m_buffer->GetRemainingLen() == 0)
		{
			// io
			ProcessIO();

			// reset msg data
			m_buffer->ResetData();
			if (buf_remain_len != 0)
			{
				std::array<char, DEFAULT_BUFLEN> buf;
				std::memcpy(buf.data(), buffer.data() + (length - buf_remain_len), buf_remain_len);
				PushData(buf, buf_remain_len);
			}
		}
		*/

		return true;
	}

	virtual void SendStream(const std::vector<BYTE> stream) final
	{
		m_ptr_sender->Send(m_conn_id, stream.data(), static_cast<int>(stream.size()));
	}

	virtual const std::string& GetUid() const = 0;
	virtual void ProcessIO() = 0;
};