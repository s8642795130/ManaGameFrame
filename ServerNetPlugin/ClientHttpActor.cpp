#include "ClientHttpActor.h"

void ClientHttpActor::Parsing(std::array<char, DEFAULT_BUFLEN>& buffer, ssize_t len)
{
	auto state = m_http_buffer->GetDataState();

	// local
	ssize_t less = len;

	switch (state)
	{
	case -1: // need more data, but length unkonw
	{
		m_http_buffer->PushData(buffer.data(), static_cast<int>(len));
		less = 0;
		m_http_buffer->DetectHeader();

		// exit
		break;
	}
	case 0: // process io
	{
		ProccessIO();
		m_http_buffer->Reset();

		// exit
		break;
	}
	case 1: // need more data, by length
	{
		ssize_t push_length = m_http_buffer->GetRemainingLen() <= len ? m_http_buffer->GetRemainingLen() : len;
		m_http_buffer->PushData(buffer.data(), static_cast<int>(push_length));
		less -= push_length;
		m_http_buffer->DetectHeader();

		// exit
		break;
	}
	case 2: // more data
	{
		auto more_len = m_http_buffer->GetMoreDataLength();
		less = more_len;
		ProccessIO();
		m_http_buffer->Reset();

		// exit
		break;
	}
	default:
		break;
	}

	if (less != 0 || m_http_buffer->ReadyToExecute())
	{
		std::array<char, DEFAULT_BUFLEN> next_buffer;
		std::memcpy(next_buffer.data(), buffer.data() + (len - less), less);
		Parsing(next_buffer, less);
	}
}

std::shared_ptr<ByteBuffer>& ClientHttpActor::GetBuffer()
{
	return m_http_buffer->GetBuffer();
}