#pragma once
#include "ILoginModule.h"
#include "../ServerNetPlugin/INetCallbackModule.h"
#include "LoginActor.h"

class LoginModule : public ILoginModule
{
private:
	// module
	std::shared_ptr<INetCallbackModule> m_callback_module;
	// actor
	std::shared_ptr<LoginActor> m_login_actor;
public:
	LoginModule(std::shared_ptr<IPluginManager> ptr) : ILoginModule(ptr)
	{
	}

	// life cycle
	virtual void Init();
	virtual void AfterInit();
};

