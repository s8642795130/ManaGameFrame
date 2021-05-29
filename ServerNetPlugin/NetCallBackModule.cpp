#include "NetCallbackModule.h"

void NetCallbackModule::AddReceiveCallBack(const int msgID, std::function<void(std::shared_ptr<IClientNetActor>)> call_func)
{
	m_ptr_callback_map->emplace(msgID, call_func);
}