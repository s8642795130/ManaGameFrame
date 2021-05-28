#include "ConfigPlugin.h"
#include "ConfigModule.h"

void ConfigPlugin::Install()
{
	// load module
	RegisterModule<IConfigModule, ConfigModule>(m_ptr_manager);
}

void ConfigPlugin::Uninstall()
{

}