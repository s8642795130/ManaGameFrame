#pragma once
#include "MessageDefine.h"

class ServerType
{
private:
	static NetMessage::ServerType m_server_type;
public:
	static void SetServerType(NetMessage::ServerType type)
	{
		m_server_type = type;
	}

	static const NetMessage::ServerType GetServerType()
	{
		return m_server_type;
	}
};