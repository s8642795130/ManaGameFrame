#include "MasterPlugin.h"

#include "MasterModule.h"

void MasterPlugin::Install()
{
	// load module
	RegisterModule<IMasterModule, MasterModule>(m_ptr_manager);
}

void MasterPlugin::Uninstall()
{

}