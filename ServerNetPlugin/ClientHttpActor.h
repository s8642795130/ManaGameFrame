#pragma once
#include "ClientNetActor.h"
#include "../Server/HttpBuffer.h"

class ClientHttpActor : public ClientNetActor
{
protected:
	std::shared_ptr<HttpBuffer> m_http_buffer;
protected:
	virtual void Parsing(std::array<char, DEFAULT_BUFLEN>& buffer, ssize_t len) override;
public:
	virtual std::shared_ptr<ByteBuffer>& GetBuffer();
};

