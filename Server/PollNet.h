#pragma once
#include <string>
#include <array>

#include <poll.h>

class PollNet
{
private:
	const int m_max_poll_count = 1024;
	int m_listen_socket = 0;
	int m_nfds = 1; // begin
	std::array<struct pollfd, 1024> m_arr_poll_fd;
	int InitSocket(std::string& ip, std::uint16_t port);
	bool HandleConnect();
	bool HandleRead(struct pollfd& fd);
	bool HandleWrite();
	bool HandleClose();
public:
	void StartPoll(std::string& ip, std::uint16_t port);
};

