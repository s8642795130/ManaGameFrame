#pragma once
#include <memory>

#include "IActor.h"
#include "IActorPimpl.h"
#include "IActorModule.h"

class Actor : public IActor
{
protected:
	std::unique_ptr<IActorPimpl> m_pimpl;
	std::shared_ptr<IActorModule> m_actor_module;
public:
	Actor() : IActor()
	{
		m_actor_module = IActorPimpl::GetActorPimpl()->GetModule<IActorModule>();
	}

	/*
	const std::unique_ptr<IActorPimpl>& GetActorPimpl()
	{
		return m_pimpl;
	}
	*/

	virtual ~Actor() = default;
};

