#pragma once
#include "ClientNetActor.h"

class ClientHttpActor : public ClientNetActor
{
protected:
	virtual void Parsing(std::array<char, DEFAULT_BUFLEN>& buffer, ssize_t len) override;
};

