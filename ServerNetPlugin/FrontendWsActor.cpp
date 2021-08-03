#include "FrontendWsActor.h"

FrontendWsActor::FrontendWsActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr_impl, ITcpServer* ptr_sender) :
	IFrontendActor(ptr_manager, ptr_impl, ptr_sender)
{

}

void FrontendWsActor::BackStream(const std::vector<char> stream)
{
	SendStream(stream);
	NextIO();
}