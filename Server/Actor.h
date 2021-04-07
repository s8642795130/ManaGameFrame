#pragma once
#include "IActor.h"
#include "IApplication.h"

class Actor : public IActor
{
protected:
	std::shared_ptr<IApplication> m_app;
public:
	Actor()
	{
		m_app = IApplication::GetPtr();
	}
	virtual ~Actor() = default;
};

