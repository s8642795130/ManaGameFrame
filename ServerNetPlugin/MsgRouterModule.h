#pragma once
#include "IMsgRouterModule.h"

class MsgRouterModule : public IMsgRouterModule
{
public:
	virtual int GetMsgRouterByClient(const std::string plugin_name, const int server_count, const IClientNetActor* client);
};

