#include "ServerControlPlugin.h"

void ServerControlPlugin::Install()
{
	// load module
	RegisterModule<ServerControlModule>(shared_from_this());
}

void ServerControlPlugin::Uninstall()
{

}