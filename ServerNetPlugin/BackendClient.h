#pragma once
#include <string>
#include <map>
#include <memory>

#include "../Server/ByteBuffer.h"

class BackendClient
{
public:
	BackendClient(std::unique_ptr<ByteBuffer>&& buffer) : m_buffer(std::move(buffer))
	{

	}

	std::string m_uid;
	std::string m_uuid;
	std::map<std::string, std::string> m_client_data;
	std::unique_ptr<ByteBuffer> m_buffer;
};