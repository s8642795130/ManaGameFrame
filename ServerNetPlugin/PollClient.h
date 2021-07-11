#pragma once
#include "IPollClient.h"

class PollClient : public IPollClient
{
public:
	virtual void SendData() override;
};

