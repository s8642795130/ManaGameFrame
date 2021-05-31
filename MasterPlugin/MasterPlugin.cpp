#include "MasterPlugin.h"

#include "MasterModule.h"

void MasterPlugin::Install()
{
	// load module
	m_ptr_manager->RegisterModule<IMasterModule, MasterModule>();
}

void MasterPlugin::Uninstall()
{

}