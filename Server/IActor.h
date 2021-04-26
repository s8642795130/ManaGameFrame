#pragma once
#include "UUID.h"

class IActor : public std::enable_shared_from_this<IActor>
{
protected:
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

