#pragma once
#include "IActor.h"
#include "IActorPimpl.h"

class Actor : public IActor
{
protected:
	std::shared_ptr<IActorPimpl> m_pimpl;
public:
	Actor() : IActor()
	{
		m_pimpl = IActorPimpl::GetActorPimpl();
	}

	const std::shared_ptr<IActorPimpl>& GetActorPimpl()
	{
		return m_pimpl;
	}

	virtual ~Actor() = default;
};

