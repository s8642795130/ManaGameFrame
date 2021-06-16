#include "LoginModule.h"
#include "../Server/BuiltInMsgDefine.h"

void LoginModule::Init()
{
	// module
	m_callback_module = m_ptr_manager->GetModule<INetCallbackModule>();
}

void LoginModule::AfterInit()
{
	// bind msg
	std::function<void(IClientNetActor&)> call_func = std::bind(&LoginModule::OnLoginOnlineCallback, this, std::placeholders::_1);
	m_callback_module->AddHTTPCallback("/login", call_func);
}

//

void LoginModule::OnLoginOnlineCallback(IClientNetActor& client)
{

}