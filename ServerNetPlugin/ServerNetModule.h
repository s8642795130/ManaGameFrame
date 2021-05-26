#pragma once
#include "IServerNetModule.h"

class ServerNetModule : public IServerNetModule
{
private:
	const int m_worker_max_events = WORKER_MAX_EVENTS;
	int m_listen_fd = -1;
	int m_epoll_fd = -1;
	std::array<epoll_event, WORKER_MAX_EVENTS> m_arr_events;
	std::map<int, std::shared_ptr<ClientNet>> m_map_clients;
	//uint32_t timeout_secs_;
	//time_t last_socket_check_;
	std::shared_ptr<IThreadPool> m_ptr_thread_pool;
	//
	std::shared_ptr<std::map<int, std::function<void(ClientDescriptor*)>>> m_ptr_callback_map;
public:
};

