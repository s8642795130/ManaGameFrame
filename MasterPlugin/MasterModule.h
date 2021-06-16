#pragma once
#include <memory>

#include "../ServerNetPlugin/IClientNetActor.h"
#include "../ServerNetPlugin/INetCallbackModule.h"
#include "../ActorPlugin/IThreadPoolModule.h"
#include "MasterActor.h"
#include "IMasterModule.h"

class MasterModule : public IMasterModule
{
private:
	// module
	std::shared_ptr<INetCallbackModule> m_callback_module;
	std::shared_ptr<IThreadPoolModule> m_thread_pool_module;

	// actor
	std::shared_ptr<MasterActor> m_master_actor;
public:
	MasterModule(std::shared_ptr<IPluginManager> ptr) : IMasterModule(ptr)
	{
	}

	// life cycle
	virtual void Init();
	virtual void AfterInit();

	// callback
	void OnServerOnlineCallback(IClientNetActor& client);
};
