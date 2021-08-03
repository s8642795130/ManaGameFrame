#pragma once
#include "IFrontendActor.h"

class FrontendWsActor : public IFrontendActor
{
public:
	FrontendWsActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr, ITcpServer* ptr_sender);
	virtual void BackStream(const std::vector<char> stream) override;
};