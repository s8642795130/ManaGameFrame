#include "LoginPlugin.h"

#include "LoginModule.h"

void LoginPlugin::Install()
{
	// load module
	m_ptr_manager->RegisterModule<ILoginModule, LoginModule>();
}

void LoginPlugin::Uninstall()
{

}