#pragma once
#include "ILoginModule.h"
#include "../ServerNetPlugin/INetCallbackModule.h"

class LoginModule : public ILoginModule
{
private:
	// module
	std::shared_ptr<INetCallbackModule> m_callback_module;
public:
	LoginModule(std::shared_ptr<IPluginManager> ptr) : ILoginModule(ptr)
	{
	}

	// life cycle
	virtual void Init();
	virtual void AfterInit();

	// callback
	void OnLoginOnlineCallback(IClientNetActor& client);
};

