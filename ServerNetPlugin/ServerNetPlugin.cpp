#include "ServerNetPlugin.h"
#include "ServerNetModule.h"
#include "ClientNetModule.h"
#include "NetCallbackModule.h"
#include "MsgRouterModule.h"
#include "NetProccessModule.h"
#include "ServerObjModule.h"

void ServerNetPlugin::Install()
{
	// load module
	m_ptr_manager->RegisterModule<IClientNetModule, ClientNetModule>();
	m_ptr_manager->RegisterModule<IMsgRouterModule, MsgRouterModule>();
	m_ptr_manager->RegisterModule<INetCallbackModule, NetCallbackModule>();
	m_ptr_manager->RegisterModule<INetProccessModule, NetProccessModule>();
	m_ptr_manager->RegisterModule<IServerNetModule, ServerNetModule>();
	m_ptr_manager->RegisterModule<IServerObjModule, ServerObjModule>();
}

void ServerNetPlugin::Uninstall()
{

}