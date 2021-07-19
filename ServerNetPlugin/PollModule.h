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
#include "IPollClient.h"
#include "../ConfigPlugin/IConfigModule.h"

class PollModule : public IPollModule
{
private:
	const int m_max_poll_count = 1024;
	int m_listen_socket = 0;
	int m_nfds = 0; // begin
	std::array<struct pollfd, 1024> m_arr_poll_fd;

	// module
	std::shared_ptr<IClientNetModule> m_client_net_module;
	std::shared_ptr<IServerObjModule> m_server_obj_module;
	std::shared_ptr<IConfigModule> m_config_module;

	// map 
	std::map<std::string, std::shared_ptr<IPollClient>> m_map_client_net; // key: server_name value: poll_client
	std::map<int, std::shared_ptr<IPollClient>> m_map_client_fd; // key: fd
protected:
	void AddClientToMapByFD(const int fd, std::shared_ptr<IPollClient> ptr_client);
	std::shared_ptr<IPollClient> GetClientFromMapByFD(const int fd);
protected:
	void InitFD();
	bool ConnectMasterServer();
	bool ConnectServer(const std::string& ip, const int port, int& fd);
	void AddSocket(int fd);
	bool HandleRead(struct pollfd& poll_fd);
	void AddClientToMap(const std::string& server_name, std::shared_ptr<IPollClient> ptr_client);
public:
	PollModule(std::shared_ptr<IPluginManager> ptr) : IPollModule(ptr)
	{
	}

	// life cycle function
	virtual void Init() override;
	virtual void AfterInit() override;

	// interface
	virtual void EventLoop() override;
	virtual bool ConnectServerWithServerName(const std::string& ip, const int port, const std::string& server_name) override;
	virtual std::shared_ptr<IPollClient> GetClientByServerName(const std::string& server_name) override;
};

