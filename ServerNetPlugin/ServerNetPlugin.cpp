#include "ServerNetPlugin.h"
#include "IServerNetModule.h"
#include "IClientNetModule.h"

void ServerNetPlugin::Install()
{
	// load module
	RegisterModule<IServerNetModule>(m_ptr_manager);
	RegisterModule<IClientNetModule>(m_ptr_manager);
}

void ServerNetPlugin::Uninstall()
{

}