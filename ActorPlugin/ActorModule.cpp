#include "ActorModule.h"
#include "ActorPimpl.h"

void ActorModule::Init()
{
}

std::unique_ptr<IActorPimpl> ActorModule::CreateActorPimpl()
{
	return std::make_unique<ActorPimpl>(m_ptr_manager);
}