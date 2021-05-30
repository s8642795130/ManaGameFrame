#include "ServerNetPlugin.h"
#include "ServerNetModule.h"
#include "ClientNetModule.h"
#include "NetCallbackModule.h"

void ServerNetPlugin::Install()
{
	// load module
	RegisterModule<IServerNetModule, ServerNetModule>(m_ptr_manager);
	RegisterModule<IClientNetModule, ClientNetModule>(m_ptr_manager);
	RegisterModule<INetCallbackModule, NetCallbackModule>(m_ptr_manager);
}

void ServerNetPlugin::Uninstall()
{

}