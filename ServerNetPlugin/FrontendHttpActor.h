#pragma once
#include "IFrontendActor.h"

class FrontendHttpActor : public IFrontendActor
{
private:
	std::shared_ptr<ByteBuffer> m_http_buffer;
protected:
	virtual void PushData(const BYTE* ptr_data, int length) override;
	virtual void ProcessIO() override;
	virtual void BackStream(const std::vector<char> stream) override;
public:
	FrontendHttpActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr_impl, ITcpServer* ptr_sender) :
		IFrontendActor(ptr_manager, ptr_impl, ptr_sender)
	{
	}
};

