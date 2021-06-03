#pragma once

namespace BuiltInMsg
{
	enum class ServerMsg : int // built-in system
	{
		NONE,
		SERVER_ONLINE,
		SERVER_OFFLINE,
		LOGIN_MSG,
		RETURN_CLIENT_MSG,
		UPDATE_CLIENT_DATA
	};

	enum class EventCode : int
	{
		NONE,
		SUCCESS,
		FAIL,
		ACCOUNT_EXIST,
		ACCOUNT_PWD_INVALID,
		ACCOUNT_LOGIN_SUCCESS,
		ACCOUNT_LOCKED
	};
}