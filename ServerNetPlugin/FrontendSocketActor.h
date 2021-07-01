#pragma once
#include "IFrontendActor.h"

class FrontendSocketActor : public IFrontendActor
{
public:
	FrontendSocketActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr);
	virtual void BackStream(std::vector<char> stream);
};

