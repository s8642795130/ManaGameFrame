#pragma once
#include "ClientNetActor.h"
#include "HttpBuffer.h"

class ClientHttpActor : public ClientNetActor
{
protected:
	std::shared_ptr<HttpBuffer> m_http_buffer;
protected:
	virtual void Parsing(std::array<char, DEFAULT_BUFLEN>& buffer, ssize_t len) override;
public:
	ClientHttpActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr) : ClientNetActor(ptr_manager, ptr)
	{
		m_http_buffer = std::make_shared<HttpBuffer>();
	}

	// interface
	virtual std::shared_ptr<ByteBuffer>& GetBuffer();
};

