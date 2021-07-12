#pragma once
#include "PollClient.h"

class PollMasterClient : public PollClient
{
public:
	PollMasterClient(std::shared_ptr<ClientPimpl> ptr_impl) : PollClient(ptr_impl)
	{
	}

	void ProcessIO();
};

