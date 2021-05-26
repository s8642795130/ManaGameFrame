#include "ServerNetPlugin.h"
#include "ServerNetModule.h"

void ServerNetPlugin::Install()
{
	// load module
	RegisterModule<ServerNetModule>(m_ptr_manager);
}

void ServerNetPlugin::Uninstall()
{

}