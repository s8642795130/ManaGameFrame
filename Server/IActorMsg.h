#pragma once
#include <string>
#include <memory>

#include "IActor.h"

class IActorMsg
{
public:
	virtual const std::string& GetReceiverActorUUID() = 0;
	virtual void Act(const IActor* ptr) = 0;
};