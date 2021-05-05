#pragma once
#include <memory>

class IConfigFile;

class ServerController
{
private:
	std::shared_ptr<IConfigFile> m_ptr_config_file;
public:
	ServerController(std::shared_ptr<IConfigFile> ptr_config_file) : m_ptr_config_file(ptr_config_file)
	{

	}

	void ConnectMaster();
	void ConnectServer();
};

