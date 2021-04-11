#include "MasterModule.h"

void MasterModule::Init()
{
	// load actor
	m_master_actor = std::make_shared<MasterActor>();
}

void MasterModule::AfterInit()
{
	// m_plugin->
	m_plugin->GetAppPtr()->AddReceiveCallBack(, this, );
}

void MasterModule::ReadyExecute()
{

}

void MasterModule::Execute()
{

}