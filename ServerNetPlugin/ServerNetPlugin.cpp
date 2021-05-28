#include "ServerNetPlugin.h"
#include "ServerNetModule.h"
#include "ClientNetModule.h"
#include "NetCallBackModule.h"

void ServerNetPlugin::Install()
{
	// load module
	RegisterModule<IServerNetModule, ServerNetModule>(m_ptr_manager);
	RegisterModule<IClientNetModule, ClientNetModule>(m_ptr_manager);
	RegisterModule<INetCallBackModule, NetCallBackModule>(m_ptr_manager);
}

void ServerNetPlugin::Uninstall()
{

}