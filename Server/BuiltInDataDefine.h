#pragma once
#include <string>
#include <vector>

#include "DataType.h"
#include "StructSchema.h"

struct ServerOnlineData
{
	std::string m_server_type;
	std::string m_server_name;
	std::string m_ip;
	int m_port;
};

struct ServerOnlineInfo
{
	std::vector<ServerOnlineData> m_vec_server;
};

struct ConnectServerOnline
{
	std::string m_server_name;
};

struct BackendMsgToClient
{
	std::string m_client_uuid;
	std::string m_uid;
	std::vector<char> m_buffer;
};

DEFINE_STRUCT_SCHEMA(ServerOnlineData,
	DEFINE_STRUCT_FIELD(m_server_type, DATA_TYPE::STRING),
	DEFINE_STRUCT_FIELD(m_server_name, DATA_TYPE::STRING),
	DEFINE_STRUCT_FIELD(m_ip, DATA_TYPE::STRING),
	DEFINE_STRUCT_FIELD(m_port, DATA_TYPE::INT)
);

DEFINE_STRUCT_SCHEMA(ServerOnlineInfo,
	DEFINE_STRUCT_FIELD(m_vec_server, DATA_TYPE::ARRAY_STRUCT)
);

DEFINE_STRUCT_SCHEMA(ConnectServerOnline,
	DEFINE_STRUCT_FIELD(m_server_name, DATA_TYPE::STRING)
);

DEFINE_STRUCT_SCHEMA(BackendMsgToClient,
	DEFINE_STRUCT_FIELD(m_client_uuid, DATA_TYPE::STRING),
	DEFINE_STRUCT_FIELD(m_buffer, DATA_TYPE::ARRAY_CHAR)
);