#include "LoginModule.h"
#include "../Server/BuiltInMsgDefine.h"

void LoginModule::Init()
{
	// module
	m_callback_module = m_ptr_manager->GetModule<INetCallbackModule>();
	// actor
	m_login_actor = std::make_shared<LoginActor>(m_ptr_manager);
}

void LoginModule::AfterInit()
{
	// bind msg
	std::shared_ptr<IBindFunc> bind_func = std::make_shared<BindFunc<LoginActor>>(m_login_actor->GetUUID(), &LoginActor::OnLoginOnlineCallback);
	m_callback_module->AddHTTPCallback("/login", bind_func);
}