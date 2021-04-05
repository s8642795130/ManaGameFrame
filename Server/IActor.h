#pragma once
#include "UUID.h"

class IActor
{
private:
	std::string m_uuid;
public:
	IActor()
	{
		m_uuid = UUID::CreateUUID();
	}

	const std::string& GetUUID()
	{
		return m_uuid;
	}
};

