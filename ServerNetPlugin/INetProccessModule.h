#pragma once
#include "..\Server\IModule.h"
#include "IClientNetActor.h"

class INetProccessModule : public IModule
{
public:
	INetProccessModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	// interface
	virtual void ProcessFrontendIO(std::shared_ptr<IClientNetActor>& client) = 0;
	virtual void ProcessBackendIO(std::shared_ptr<IClientNetActor>& client) = 0;
	virtual void ProcessFrontendUnknowMsg(std::shared_ptr<IClientNetActor>& client) = 0;
	virtual void ProcessServerBackendIO(std::shared_ptr<IClientNetActor>& client) = 0;
	virtual void ProcessRPCIO(std::shared_ptr<IClientNetActor>& client) = 0;
	virtual void ProcessMasterIO(std::shared_ptr<IClientNetActor>& client) = 0;
};