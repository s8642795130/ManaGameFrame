#include "LoginModule.h"
#include "../Server/BuiltInMsgDefine.h"

void LoginModule::Init()
{
	// module
	m_callback_module = m_ptr_manager->GetModule<INetCallbackModule>();
	m_thread_pool_module = m_ptr_manager->GetModule<IThreadPoolModule>();

	// actor
	m_login_actor = std::make_shared<LoginActor>(m_ptr_manager);
}

void LoginModule::AfterInit()
{
	// add actor to thread poll
	m_thread_pool_module->AddActorToThreadCell(m_login_actor);

	// bind msg
	std::shared_ptr<IHttpBindFunc> bind_func = std::make_shared<HttpBindFunc<LoginActor>>(m_login_actor->GetUUID(), &LoginActor::OnLoginOnlineCallback);
	m_callback_module->AddHttpCallback("/login", bind_func);
}