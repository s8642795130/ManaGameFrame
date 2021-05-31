#include "ActorPlugin.h"
#include "ThreadPoolModule.h"
#include "ActorModule.h"

void ActorPlugin::Install()
{
	// load module
	m_ptr_manager->RegisterModule<IActorModule, ActorModule>();
	m_ptr_manager->RegisterModule<IThreadPoolModule, ThreadPoolModule>();
}

void ActorPlugin::Uninstall()
{

}