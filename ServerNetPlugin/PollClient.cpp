#include "PollClient.h"
#include "../Server/UnpackNetMsg.h"
#include "../Server/BuiltInMsgDefine.h"
#include "../ActorPlugin/ActorMsg.h"
#include "IFrontendActor.h"

std::shared_ptr<ByteBuffer> PollClient::GetBuffer()
{
	return m_buffer;
}

void PollClient::SendData(const int major, const int minor, std::vector<char> value)
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
	send(m_fd, temp_data.data(), HEADER_LENGTH + length, 0);
}

void PollClient::PushData(const char* ptr_data, ssize_t length)
{
	auto buf_remain_len = length;

	if (m_buffer->GetHeaderStatus() == false)
	{
		// is not read length
		auto remain_len = m_buffer->GetRemainingLen();
		auto len_buf = remain_len < length ? remain_len : length;

		//
		m_buffer->RecvHeader(ptr_data, len_buf);
		buf_remain_len -= static_cast<int>(len_buf);

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
					PushData(ptr_data + (length - buf_remain_len), buf_remain_len);
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
		m_buffer->PushData(ptr_data + (length - buf_remain_len), push_data_len);
		buf_remain_len -= static_cast<int>(push_data_len);
	}

	if (m_buffer->GetRemainingLen() == 0)
	{
		// io
		ProcessIO();

		// reset msg data
		m_buffer->ResetData();
		if (buf_remain_len != 0)
		{
			PushData(ptr_data + (length - buf_remain_len), buf_remain_len);
		}
	}
}

void PollClient::ProcessIO()
{
	// there is two situations, case 1: back msg to client; case 2: change client data
	if (m_buffer->GetMajorId() != static_cast<int>(BuiltInMsg::ServerMsg::RETURN_CLIENT_MSG))
	{
		// unpack msg
		BackendMsgToClient backend_msg;
		UnpackStructForEachField(backend_msg, m_buffer);

		// find login uid
		auto client_uuid = m_client_impl->m_client_net_module->GetLoginClient(backend_msg.m_client_uid);

		if (client_uuid != "")
		{
			// send to client
			std::unique_ptr<IActorMsg> ptr = CreateActorMsg("", client_uuid, &IFrontendActor::BackStream, std::move(backend_msg.m_buffer));
			m_client_impl->m_thread_pool_module->AddActorMsgToThreadCell(ptr);
		}
	}
	else if (m_buffer->GetMajorId() != static_cast<int>(BuiltInMsg::ServerMsg::UPDATE_CLIENT_DATA))
	{
		// unpack msg
		UpdateClient update_client;
		UnpackStructForEachField(update_client, m_buffer);

		// find client uid
		auto client_uuid = m_client_impl->m_client_net_module->GetLoginClient(update_client.m_client_uid);
		if (client_uuid != "")
		{
			std::unique_ptr<IActorMsg> ptr = CreateActorMsg("", client_uuid, &IFrontendActor::UpdateClientData, std::move(update_client.m_data_key), std::move(update_client.m_data_value), update_client.m_update_type);
			m_client_impl->m_thread_pool_module->AddActorMsgToThreadCell(ptr);
		}
	}
	else
	{
		// error...
		std::perror("ProcessBackendIO: error");
	}
}