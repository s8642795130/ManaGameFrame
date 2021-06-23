#include <functional>

#include "MasterModule.h"
#include "../Server/BuiltInMsgDefine.h"
#include "../Server/BuiltInDataDefine.h"
#include "../ActorPlugin/ActorMsg.h"

void MasterModule::Init()
{
	// module
	m_callback_module = m_ptr_manager->GetModule<INetCallbackModule>();
	m_thread_pool_module = m_ptr_manager->GetModule<IThreadPoolModule>();

	// load actor
	m_master_actor = std::make_shared<MasterActor>(m_ptr_manager);
}

void MasterModule::AfterInit()
{
	// add actor to thread poll
	m_thread_pool_module->AddActorToThreadCell(m_master_actor);

	// bind msg
	std::shared_ptr<IBindFunc> bind_func = std::make_shared<BindFunc<MasterActor>>(m_master_actor->GetUUID(), &MasterActor::OnServerOnlineCallback);
	m_callback_module->AddReceiveCallback(static_cast<int>(BuiltInMsg::ServerMsg::SERVER_ONLINE), bind_func);
}