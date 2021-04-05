#pragma once
#include <string>

class PollNet
{
private:
	int InitSocket(std::string& ip, int port);
public:
	void StartPoll(std::string& ip, int port);
};

