#pragma once
#include <string>
#include <vector>

#include "DataType.h"
#include "StructSchema.h"

struct ServerOnlineData
{
	std::string m_server_name;
};

struct ServerOnlineInfo
{
	std::vector<ServerOnlineData> m_vec_server;
};

struct ConnectServerOnline
{
	std::string m_server_name;
};

struct FrontendToBackendMsg
{
	std::string m_client_uid;
	std::map<std::string, std::string> m_client_data;
	int m_major_id;
	int m_minor_id;
	std::vector<char> m_stream;
};

struct BackendMsgToClient
{
	int m_msg_type;
	std::string m_client_uid;
	std::vector<char> m_buffer;
};

struct UpdateClient
{
	std::string m_client_uid;
	std::string m_data_key;
	int m_update_type;
	std::string m_data_value;
};

struct RPCMsgData
{
	int m_major_id;
	int m_minor_id;
	std::vector<char> m_stream;
};

DEFINE_STRUCT_SCHEMA(ServerOnlineData,
	DEFINE_STRUCT_FIELD(m_server_name, DATA_TYPE::STRING)
);

DEFINE_STRUCT_SCHEMA(ServerOnlineInfo,
	DEFINE_STRUCT_FIELD(m_vec_server, DATA_TYPE::ARRAY_STRUCT)
);

DEFINE_STRUCT_SCHEMA(ConnectServerOnline,
	DEFINE_STRUCT_FIELD(m_server_name, DATA_TYPE::STRING)
);

DEFINE_STRUCT_SCHEMA(FrontendToBackendMsg,
	DEFINE_STRUCT_FIELD(m_client_uid, DATA_TYPE::STRING),
	DEFINE_STRUCT_FIELD(m_client_data, DATA_TYPE::MAP_STRING),
	DEFINE_STRUCT_FIELD(m_major_id, DATA_TYPE::INT),
	DEFINE_STRUCT_FIELD(m_minor_id, DATA_TYPE::INT),
	DEFINE_STRUCT_FIELD(m_stream, DATA_TYPE::ARRAY_CHAR)
);

DEFINE_STRUCT_SCHEMA(BackendMsgToClient,
	DEFINE_STRUCT_FIELD(m_msg_type, DATA_TYPE::INT),
	DEFINE_STRUCT_FIELD(m_client_uid, DATA_TYPE::STRING),
	DEFINE_STRUCT_FIELD(m_buffer, DATA_TYPE::ARRAY_CHAR)
);

DEFINE_STRUCT_SCHEMA(UpdateClient,
	DEFINE_STRUCT_FIELD(m_client_uid, DATA_TYPE::STRING),
	DEFINE_STRUCT_FIELD(m_data_key, DATA_TYPE::STRING),
	DEFINE_STRUCT_FIELD(m_update_type, DATA_TYPE::INT),
	DEFINE_STRUCT_FIELD(m_data_value, DATA_TYPE::STRING)
);

DEFINE_STRUCT_SCHEMA(RPCMsgData,
	DEFINE_STRUCT_FIELD(m_major_id, DATA_TYPE::INT),
	DEFINE_STRUCT_FIELD(m_minor_id, DATA_TYPE::INT),
	DEFINE_STRUCT_FIELD(m_stream, DATA_TYPE::ARRAY_CHAR)
);