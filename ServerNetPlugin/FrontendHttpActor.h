#pragma once
#include "IFrontendActor.h"

class FrontendHttpActor : public IFrontendActor, public std::enable_shared_from_this<FrontendHttpActor>
{
private:
	IHttpServer* m_ptr_http_sender;
public:
	FrontendHttpActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr, IHttpServer* ptr_sender);
	virtual void BackStream(const std::vector<char> stream) override;
	virtual void SendStream(const std::vector<char> stream) override;
	virtual void SendData(const int major, const int minor, std::vector<char> value) override;
	virtual void ProcessIO() override;
};

