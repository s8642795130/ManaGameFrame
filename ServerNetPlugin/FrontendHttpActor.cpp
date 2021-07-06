#include "FrontendHttpActor.h"
#include "../Server/StringDefine.h"

bool FrontendHttpActor::PushData(const BYTE* ptr_data, int length)
{
	/*
	auto state = m_http_buffer->GetDataState();

	// local
	bool pass_by = true;
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
		ProcessIO();
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
		ProcessIO();
		m_http_buffer->Reset();

		// exit
		break;
	}
	case 3:
	{
		std::perror(HTTP_PACKAGE_ERROR);
		ClientClose();
		pass_by = false;
		break;
	}
	case 4:
	{
		std::perror(HTTP_PACKAGE_ERROR);
		ClientClose();
		pass_by = false;
		break;
	}
	default:
		break;
	}

	if (pass_by && (less != 0 || m_http_buffer->ReadyToExecute()))
	{
		std::array<char, DEFAULT_BUFLEN> next_buffer;
		std::memcpy(next_buffer.data(), buffer.data() + (len - less), less);
		Parsing(next_buffer, less);
	}
	*/
	return true;
}

void FrontendHttpActor::ProcessIO()
{

}

void FrontendHttpActor::BackStream(const std::vector<char> stream)
{

}