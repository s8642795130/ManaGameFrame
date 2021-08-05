#include "FrontendHttpActor.h"

FrontendHttpActor::FrontendHttpActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr_impl, IHttpServer* ptr_sender) :
	IFrontendActor(ptr_manager, ptr_impl, nullptr)
{
	m_ptr_http_sender = ptr_sender;
}

void FrontendHttpActor::BackStream(const std::vector<char> stream)
{
	SendStream(stream);
	NextIO();
}

void FrontendHttpActor::SendStream(const std::vector<char> stream)
{
	// header
	THeader header[] = { {"Content-Type", "text/plain"}, {"Content-Length", std::to_string(stream.size()).c_str()} };
	int header_count = sizeof(header) / sizeof(THeader);

	// sender
	m_ptr_http_sender->SendResponse(m_conn_id,
		HSC_OK,
		nullptr, // "HP Http Server OK",
		header, header_count,
		(const BYTE*)(LPCSTR)stream.data(),
		static_cast<int>(stream.size()));

	// test code
	m_ptr_http_sender->Disconnect(m_conn_id);
}

void FrontendHttpActor::SendData(const int major, const int minor, std::vector<char> value)
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

	SendStream(temp_data);
}

void FrontendHttpActor::ProcessIO()
{
	// int majorId = m_buffer->GetMajorId();
	auto map_callback = m_client_impl->m_callback_module->GetHTTPCallbackMap();

	// check
	// if (map_callback.find(majorId) != std::cend(map_callback))
	if (map_callback.find("/login") != std::cend(map_callback))
	{
		auto callback = map_callback["/login"];
		callback->Trigger(m_client_impl->m_thread_pool_module, shared_from_this());
	}
}