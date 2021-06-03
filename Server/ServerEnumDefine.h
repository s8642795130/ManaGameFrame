#pragma once

namespace EnumDefine
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

	enum class PushMsgType : int
	{
		RESPONSE,
		PUSH
	};

	enum class UpdateClientDataType : int
	{
		MODIFY,
		DEL
	};
}