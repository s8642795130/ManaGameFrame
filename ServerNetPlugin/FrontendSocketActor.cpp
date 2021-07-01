#include "FrontendSocketActor.h"

FrontendSocketActor::FrontendSocketActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr_impl) :
	IFrontendActor(ptr_manager, ptr_impl)
{

}

void FrontendSocketActor::BackStream(std::vector<char> stream)
{
	SendStream(stream);
	NextIO();
}