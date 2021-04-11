#pragma once

namespace NetMessage
{
	enum class EventCode : int
	{
		SUCCESS,
		FAIL,
		ACCOUNT_EXIST,
		ACCOUNT_PWD_INVALID,
		ACCOUNT_LOGIN_SUCCESS,
		ACCOUNT_LOCKED
	};

	enum class ServerMsg : int
	{
		SERVER_ONLINE,
		SERVER_OFFLINE
	};

	enum class ServerType : int
	{
		FRONTEND,
		BACKEND
	};
}