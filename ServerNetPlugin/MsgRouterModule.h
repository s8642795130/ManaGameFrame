#pragma once
#include "IMsgRouterModule.h"
#include "IFrontendActor.h"

class MsgRouterModule : public IMsgRouterModule
{
private:
	std::map<std::string, std::function<int(const int, const IFrontendActor&)>> m_router_func;
protected:
	void initRouterFunc();
	int DefaultRouter(const int server_count, const IFrontendActor& client);
public:
	MsgRouterModule(std::shared_ptr<IPluginManager> ptr) : IMsgRouterModule(ptr)
	{
	}

	// life cycle
	virtual void Init();

	// interface
	virtual int GetConnectorIndexByClient(const int server_count, const std::string& uid);
	virtual int GetMsgRouterByClient(const std::string plugin_name, const int server_count, const IFrontendActor& client);
	virtual int GetMsgRouterByString(const std::string plugin_name, const int server_count, const std::string& router);
};

