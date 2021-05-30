#include "ActorModule.h"
#include "ActorPimpl.h"

std::shared_ptr<IActorPimpl> IActorPimpl::m_pimpl;

void ActorModule::Init()
{
	std::shared_ptr<ActorPimpl> actor_pimpl{ std::make_shared<ActorPimpl>(m_ptr_manager) };
	IActorPimpl::SetActorPimpl(actor_pimpl);
}