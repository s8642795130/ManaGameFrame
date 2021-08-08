#pragma once
#include "ILoginModule.h"
#include "../ServerNetPlugin/INetCallbackModule.h"
#include "../ActorPlugin/IThreadPoolModule.h"
#include "LoginActor.h"

class LoginModule : public ILoginModule
{
private:
	// module
	std::shared_ptr<INetCallbackModule> m_callback_module;
	std::shared_ptr<IThreadPoolModule> m_thread_pool_module;
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

