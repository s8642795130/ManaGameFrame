#include "ActorPlugin.h"
#include "ThreadPoolModule.h"

void ActorPlugin::Install()
{
	// load module
	RegisterModule<IThreadPoolModule, ThreadPoolModule>(m_ptr_manager);
}

void ActorPlugin::Uninstall()
{

}