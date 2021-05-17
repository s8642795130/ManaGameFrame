#include <iostream>

#include "MasterActor.h"
#include "../Server/GameMessageData.h"
#include "../Server/PackageNetMsg.h"

void MasterActor::ServerOnline(int test, int c)
{
	std::cout << "Server is online !" << std::endl;

	ServerOnlineInfo server_online_info;
	std::vector<char> buffer;
	PackageStructForEachField(server_online_info, buffer);
}