#include "ConfigPlugin.h"
#include "ConfigModule.h"

void ConfigPlugin::Install()
{
	// load module
	m_ptr_manager->RegisterModule<IConfigModule, ConfigModule>();
}

void ConfigPlugin::Uninstall()
{

}