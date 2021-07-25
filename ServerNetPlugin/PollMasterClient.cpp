#include "PollMasterClient.h"

void PollMasterClient::ProcessIO()
{
	int majorId = m_buffer->GetMajorId();
	const auto& map_callback = m_client_impl->m_callback_module->GetMasterCallbackMap();

	// check
	if (map_callback.find(majorId) != std::cend(map_callback))
	{
		auto callback = map_callback.at(majorId);
		callback(*this);
	}
}