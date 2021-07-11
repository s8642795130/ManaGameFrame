#pragma once
#include "IPollModule.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <poll.h>

#include "IClientNetModule.h"
#include "IServerObjModule.h"
#include "MasterClientActor.h"
#include "IPollClient.h"

class PollModule : public IPollModule
{
private:
	const int m_max_poll_count = 1024;
	int m_listen_socket = 0;
	int m_nfds = 1; // begin
	std::array<struct pollfd, 1024> m_arr_poll_fd;
	// module
	std::shared_ptr<IClientNetModule> m_client_net_module;
	std::shared_ptr<IServerObjModule> m_server_obj_module;
	// actor
	std::shared_ptr<MasterClientActor> m_master_client_actor;
	// map 
	std::map<std::string, std::shared_ptr<IPollClient>> m_map_client_net;
protected:
	void InitFD();
	bool ConnectServer(const std::string& ip, const int port, const std::string& server_name);
	void AddSocket(int fd);
	void AddActorToMap(int fd, const std::string& server_name);
	bool HandleRead(struct pollfd& poll_fd);
public:
	PollModule(std::shared_ptr<IPluginManager> ptr) : IPollModule(ptr)
	{
	}

	// life cycle function
	virtual void Init() override;

	// interface
	virtual void StartPoll();
	virtual void SendBufferByServerName(std::vector<char> buffer, const std::string& server_name);
};

