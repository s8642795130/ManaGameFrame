#include "ServerNetPlugin.h"
#include "ServerNetModule.h"
#include "ClientNetModule.h"
#include "NetCallbackModule.h"
#include "MsgRouterModule.h"
#include "ServerObjModule.h"
#include "PollModule.h"

void ServerNetPlugin::Install()
{
	// load module
	m_ptr_manager->RegisterModule<IClientNetModule, ClientNetModule>();
	m_ptr_manager->RegisterModule<IMsgRouterModule, MsgRouterModule>();
	m_ptr_manager->RegisterModule<INetCallbackModule, NetCallbackModule>();
	m_ptr_manager->RegisterModule<IServerNetModule, ServerNetModule>();
	m_ptr_manager->RegisterModule<IServerObjModule, ServerObjModule>();
	m_ptr_manager->RegisterModule<IPollModule, PollModule>();
}

void ServerNetPlugin::Uninstall()
{

}