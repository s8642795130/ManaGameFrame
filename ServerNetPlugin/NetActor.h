#pragma once
#include "INetActor.h"
#include "../Server/ByteBuffer.h"
#include "ClientPimpl.h"
#include "HPSocket.h"

class NetActor : public INetActor
{
protected:
	ITcpServer* m_ptr_sender = nullptr;
	// impl
	std::shared_ptr<ClientPimpl> m_client_impl;
	//
	std::shared_ptr<ByteBuffer> m_buffer;
public:
	NetActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr_impl, ITcpServer* ptr_sender) :
		INetActor(ptr_manager),
		m_ptr_sender(ptr_sender),
		m_client_impl(ptr_impl),
		m_buffer(std::make_shared<ByteBuffer>())
	{
	}

	unsigned long m_conn_id;

	virtual const unsigned long GetSid() const override
	{
		return m_conn_id;
	}

	virtual std::shared_ptr<ByteBuffer>& GetBuffer() override
	{
		return m_buffer;
	}

	virtual bool PushData(const BYTE* ptr_data, int length) override
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

	virtual void SendStream(const std::vector<char> stream) override
	{
		m_ptr_sender->Send(m_conn_id, reinterpret_cast<const BYTE*>(stream.data()), static_cast<int>(stream.size()));
	}

	virtual void SendData(const int major, const int minor, std::vector<char> value) override
	{
		// get length
		int length = static_cast<int>(value.size());

		//
		std::vector<char> temp_data(HEADER_LENGTH + length);
		std::memcpy(temp_data.data(), &major, sizeof(MAJOR_LENGTH)); // copy major
		std::memcpy(temp_data.data() + MAJOR_LENGTH, &minor, sizeof(MINOR_LENGTH)); // copy minor
		std::memcpy(temp_data.data() + MAJOR_LENGTH + MINOR_LENGTH, &length, sizeof(INT_LENGTH)); // copy length
		if (length != 0)
		{
			std::memcpy(temp_data.data() + HEADER_LENGTH, value.data(), length);
		}
		m_ptr_sender->Send(m_conn_id, reinterpret_cast<const BYTE*>(temp_data.data()), HEADER_LENGTH + length);
	}

	virtual void ClientClose() override
	{
		m_ptr_sender->Disconnect(m_conn_id);

		std::cout << "[-] connection " << std::endl;
	}
};