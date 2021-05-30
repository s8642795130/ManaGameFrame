#pragma once

namespace NetServerType
{
	enum class ServerType : int
	{
		NONE,
		FRONTEND,
		BACKEND,
		MASTER
	};

	enum class ClientType : int
	{
		NONE,
		CLIENT,
		FRONTEND,
		BACKEND,
		MASTER,
		FAKE
	};
}