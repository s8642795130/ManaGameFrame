#pragma once
#include "../Server/IModule.h"
#include "IClientNetActor.h"

class INetProccessModule : public IModule
{
public:
	INetProccessModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	// interface
	/*
	virtual void ProcessBackendIO(IClientNetActor& client) = 0;
	virtual void ProcessFrontendUnknowMsg(std::shared_ptr<IClientNetActor> client) = 0;
	virtual void ProcessServerBackendIO(IClientNetActor& client) = 0;
	virtual void ProcessBackendUnknowMsg(IClientNetActor& client) = 0;
	virtual void ProcessRPCIO(IClientNetActor& client) = 0;
	virtual void ProcessMasterIO(IClientNetActor& client) = 0;
	virtual void ProcessTempIO(std::shared_ptr<IClientNetActor> client) = 0;
	virtual void ProcessHttpIO(IClientNetActor& client) = 0;
	*/
};