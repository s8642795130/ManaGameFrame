#include "BackendPollActor.h"

void BackendPollActor::SendStream(const std::vector<char> stream)
{
	send(m_conn_id, stream.data(), stream.size(), 0);
}

void BackendPollActor::SendData(const int major, const int minor, std::vector<char> value)
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
	send(m_conn_id, temp_data.data(), HEADER_LENGTH + length, 0);
}