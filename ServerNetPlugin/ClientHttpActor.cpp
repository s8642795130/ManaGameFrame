#include "ClientHttpActor.h"

void ClientHttpActor::Parsing(std::array<char, DEFAULT_BUFLEN>& buffer, ssize_t len)
{
	auto state = m_http_buffer->GetDataState();

	switch (state)
	{
	case -1:
		break;
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	default:
		break;
	}
}