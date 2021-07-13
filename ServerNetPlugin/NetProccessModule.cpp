#include "NetProccessModule.h"
#include "BackendClient.h"
#include "../Server/BuiltInMsgDefine.h"
#include "../Server/BuiltInDataDefine.h"
#include "../Server/UnpackNetMsg.h"
#include "../Server/PackageNetMsg.h"
#include "../ActorPlugin/ActorMsg.h"

void NetProccessModule::Init()
{
	m_router_module = m_ptr_manager->GetModule<IMsgRouterModule>();
	m_callback_module = m_ptr_manager->GetModule<INetCallbackModule>();
	m_server_obj_module = m_ptr_manager->GetModule<IServerObjModule>();
	m_config_module = m_ptr_manager->GetModule<IConfigModule>();
	m_client_net_module = m_ptr_manager->GetModule<IClientNetModule>();
	m_thread_pool_module = m_ptr_manager->GetModule<IThreadPoolModule>();
}

/*

void NetProccessModule::ProcessTempIO(std::shared_ptr<IClientNetActor> client)
{
	int majorId = client->GetBuffer()->GetMajorId();
	auto map_callback = m_callback_module->GetReceiveCallbackMap();

	// check
	if (map_callback.find(majorId) != std::cend(map_callback))
	{
		auto callback = map_callback[majorId];
		callback->Trigger(m_thread_pool_module, client);
	}
}

void NetProccessModule::ProcessHttpIO(IClientNetActor& client)
{
	int majorId = client.GetBuffer()->GetMajorId();
	auto map_callback = m_callback_module->GetHTTPCallbackMap();

	// check
	if (map_callback.find(majorId) != std::cend(map_callback))
	{
		auto callback = map_callback[majorId];
		callback->Trigger(m_thread_pool_module, client);
	}
}
*/