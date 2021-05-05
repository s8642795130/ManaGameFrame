#include "ServerController.h"
#include "ConfigFile.h"
#include "ServerObj.h"

void ServerController::ConnectMaster()
{
	// get master server ip and port
	auto vec_server_name = m_ptr_config_file->GetServersByType("master");
	auto server_data = m_ptr_config_file->GetServerDataByName(vec_server_name[0]);

	std::shared_ptr<ServerObj> server_obj{ std::make_shared<ServerObj>() };
	server_obj->ConnectServer(server_data->m_server_ip, server_data->m_port);
}

void ServerController::ConnectServer()
{
	
}