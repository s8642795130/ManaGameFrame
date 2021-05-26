#include "ActorPlugin.h"
#include "IThreadPoolModule.h"

void ActorPlugin::Install()
{
	// load module
	RegisterModule<IThreadPoolModule>(m_ptr_manager);
}

void ActorPlugin::Uninstall()
{

}