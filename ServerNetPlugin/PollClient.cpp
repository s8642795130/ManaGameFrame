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

void PollClient::ProcessIO()
{
	switch (m_client_type)
	{
	case EnumDefine::ClientType::FRONTEND:
		ProcessBackendIO();
		break;
	case EnumDefine::ClientType::BACKEND:
		break;
	default:
		break;
	}
}

void PollClient::ProcessBackendIO()
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
			m_thread_pool_module->AddActorMsgToThreadCell(ptr);
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
			ptr_client->UpdateClientData(update_client.m_data_key, update_client.m_data_value, update_client.m_update_type);
		}
	}
	else
	{
		// error...
		std::perror("ProcessBackendIO: error");
	}
}