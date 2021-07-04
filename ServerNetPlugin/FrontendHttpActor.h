#pragma once
#include "IFrontendActor.h"
#include "HttpBuffer.h"

class FrontendHttpActor : public IFrontendActor
{
private:
	std::shared_ptr<HttpBuffer> m_http_buffer;
protected:
	virtual void Parsing(std::array<char, DEFAULT_BUFLEN>& buffer, ssize_t len) override;
	virtual void ProcessIO() override;
	virtual void BackStream(std::vector<char> stream) override;
public:
	FrontendHttpActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr_impl) :
		IFrontendActor(ptr_manager, ptr_impl)
	{
	}
};

