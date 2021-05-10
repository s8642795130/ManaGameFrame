#pragma once

namespace NetMessage
{
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

	enum class ServerMsg : int
	{
		NONE,
		SERVER_ONLINE,
		SERVER_OFFLINE,
		FRONTEND_MSG
	};
}

class NetMsgDefine
{
private:
	static std::multimap<std::string, int> m_map_msg;
public:
	static const std::multimap<std::string, int> GetNetMsg()
	{
		return m_map_msg;
	}
};