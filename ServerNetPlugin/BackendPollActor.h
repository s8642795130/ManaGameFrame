#pragma once
#include "IBackendActor.h"

class BackendPollActor : public IBackendActor
{
public:
	virtual void SendStream(const std::vector<char> stream) override;
	virtual void SendData(const int major, const int minor, std::vector<char> value) override;
};

