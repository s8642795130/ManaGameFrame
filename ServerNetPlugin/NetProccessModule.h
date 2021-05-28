#pragma once
#include "INetProccessModule.h"

class NetProccessModule : public INetProccessModule
{
public:
	NetProccessModule(std::shared_ptr<IPluginManager> ptr) : INetProccessModule(ptr)
	{
	}

	// interface
	virtual void ProcessFrontendIO(std::shared_ptr<IClientNetActor>& client);
	virtual void ProcessBackendIO(std::shared_ptr<IClientNetActor>& client);
	virtual void ProcessFrontendUnknowMsg(std::shared_ptr<IClientNetActor>& client;
	virtual void ProcessServerBackendIO(std::shared_ptr<IClientNetActor>& client);
	virtual void ProcessRPCIO(std::shared_ptr<IClientNetActor>& client);
	virtual void ProcessMasterIO(std::shared_ptr<IClientNetActor>& client);
};

