#include "MasterPlugin.h"

#include "MasterModule.h"

void MasterPlugin::Install()
{
	// load module
	RegisterModule<MasterModule>(shared_from_this());
}

void MasterPlugin::Uninstall()
{

}