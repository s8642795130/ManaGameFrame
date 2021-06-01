#pragma once
#include <string>
#include <map>
#include <memory>

#include "../Server/ByteBuffer.h"

class BackendClient
{
public:
	std::string m_uid;
	std::string m_uuid;
	std::map<std::string, std::string> m_client_data;
	std::shared_ptr<ByteBuffer> m_buffer;
};