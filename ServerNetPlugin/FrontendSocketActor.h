#pragma once
#include "IFrontendActor.h"

class FrontendSocketActor : public IFrontendActor
{
public:
	FrontendSocketActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr, ITcpServer* ptr_sender);
	virtual void BackStream(const std::vector<char> stream) override;
};

