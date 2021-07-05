#include "FrontendSocketActor.h"

FrontendSocketActor::FrontendSocketActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr_impl, ITcpServer* ptr_sender) :
	IFrontendActor(ptr_manager, ptr_impl, ptr_sender)
{

}

void FrontendSocketActor::BackStream(const std::vector<BYTE> stream)
{
	SendStream(stream);
	NextIO();
}