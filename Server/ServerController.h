#pragma once
#include <memory>
#include <string>
#include <map>

class IConfigFile;
class IThreadPool;

class ServerController
{
private:
	std::shared_ptr<IConfigFile> m_ptr_config_file;
	std::shared_ptr<IThreadPool> m_ptr_thread_pool;
	static std::map<std::string, std::string> m_map_server; // key: server_name, value: uuid

	void SaveServerUUID(const std::string& server_name, const std::string& uuid);
public:
	ServerController(std::shared_ptr<IConfigFile> ptr_config_file, std::shared_ptr<IThreadPool> ptr_thread_pool) : m_ptr_config_file(ptr_config_file), m_ptr_thread_pool(ptr_thread_pool)
	{

	}

	void ConnectMaster();
	void ConnectServer();

	static const std::map<std::string, std::string>& GetServerMap()
	{
		return m_map_server;
	}
};