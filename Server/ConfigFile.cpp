#include "ConfigFile.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <regex>

/// <summary>
/// �ָ��ַ���
/// </summary>
/// <param name="s"></param>
/// <returns></returns>
void ConfigFile::StringSplit(const std::string& str, const char split, std::vector<std::string>& res)
{
	if (str == "")
		return;
	//���ַ���ĩβҲ����ָ����������ȡ���һ��
	std::string strs = str + split;
	size_t pos = strs.find(split);

	// ���Ҳ����������ַ��������������� npos
	while (pos != strs.npos)
	{
		std::string temp = strs.substr(0, pos);
		res.push_back(temp);
		//ȥ���ѷָ���ַ���,��ʣ�µ��ַ����н��зָ�
		strs = strs.substr(pos + 1, strs.size());
		pos = strs.find(split);
	}
}

void ConfigFile::SetServerName(const std::string& server_name)
{
	m_server_name = server_name;
}

/// <summary>
/// ��ȡ�����������ļ�
/// </summary>
/// <returns></returns>
bool ConfigFile::ReadServerConfigFile()
{
	// �ļ�����ʽ���ļ�
	std::ifstream in{ m_config_file_name, std::ifstream::in };

	if (!in)
	{
		std::cerr << "config file error" << std::endl;
		std::exit(1);
	}

	// �����ļ��� ��ʼ����� ������
	auto in_iter = std::istream_iterator<std::string>(in);
	auto end_iter = std::istream_iterator<std::string>{};

	// ȷ�������ļ��ж�����
	std::size_t size = std::distance(in_iter, end_iter);

	// distance ���޸ĵ�����λ�� ʹ��clear��seek��������ָ��ԭ
	in.clear();
	in.seekg(0);

	// �������ļ���Ϣ���Ƶ� config_str
	std::vector<std::string> config_str{ size };
	std::copy(std::istream_iterator<std::string>(in), end_iter, std::begin(config_str));

	// ����������Ϣ
	AnalyseConfigStr(config_str);
	AnalysePluginList();

	return true;
}

/// <summary>
/// ���������ַ���
/// </summary>
/// <param name="config_str"></param>
void ConfigFile::AnalyseConfigStr(const std::vector<std::string>& config_str)
{
	// ������const �������Բ����Ի�ȡ������ std::vector<std::string>::iterator iter = vec.begin();

	std::string temp_server_name;

	std::vector<std::string>::const_iterator c_iter = std::cbegin(config_str);
	while (c_iter != std::cend(config_str))
	{
		std::vector<std::string> param;
		StringSplit(*c_iter, '|', param);

		if (param.size() == 4)
		{
			// ServerData
			std::shared_ptr<ServerData> server_data = std::make_shared<ServerData>();
			server_data->m_server_type = param[0];
			server_data->m_server_name = param[1];
			server_data->m_server_ip = param[2];
			server_data->m_port = std::atoi(param[3].c_str());
			m_vec_server.push_back(server_data);

			// set temp server name
			temp_server_name = server_data->m_server_name;

			// server config
			auto it = m_server_config.find(server_data->m_server_name);
			if (it == std::cend(m_server_config))
			{
				m_server_config[server_data->m_server_name] = server_data;
			}
			else
			{
				std::perror("duplicate server names in the configuration file");
				std::exit(0);
			}

			// plugin list
			std::vector<std::shared_ptr<PluginData>> vec_plugin_data;
			m_plugin_list[server_data->m_server_name] = vec_plugin_data;

			// type list
			auto type_it = m_type_config.find(server_data->m_server_name);
			if (type_it == std::cend(m_type_config))
			{
				std::vector<std::shared_ptr<ServerData>> vec_server_list;
				vec_server_list.push_back(server_data);
				m_type_config[param[0]] = vec_server_list;
			}
			else
			{
				m_type_config[param[0]].push_back(server_data);
			}
		}
		else
		{
			// plugin data
			std::shared_ptr<PluginData> plugin_data = std::make_shared<PluginData>();
			plugin_data->m_plugin = param[0];
			plugin_data->m_plugin_name = param[1];
			m_vec_plugin.push_back(plugin_data);

			// plugin list
			m_plugin_list[temp_server_name].push_back(plugin_data);
		}
		++c_iter;
	}
}

void ConfigFile::AnalysePluginList()
{
	for (auto& server_item : m_plugin_list)
	{
		for (auto& plugin_item : server_item.second)
		{
			auto server_name = server_item.first;

			auto it = m_plugin_config.find(plugin_item->m_plugin);
			if (it != std::cend(m_plugin_config))
			{
				std::vector<std::shared_ptr<ServerData>> vec_server_list;
				m_plugin_config[plugin_item->m_plugin] = vec_server_list;
			}

			// check server data is in vec
			auto vec_list = m_plugin_config[plugin_item->m_plugin];
			auto find_it = std::find_if(std::begin(vec_list), std::end(vec_list), [server_name](const auto& item) -> bool
				{
					return (item->m_server_name == server_name);
				}
			);
			if (find_it != std::cend(vec_list))
			{
				m_plugin_config[plugin_item->m_plugin].push_back(m_server_config[server_item.first]);
			}
		}
	}
}

/// <summary>
/// ���ݲ������ȡ���а��������Ͳ���ķ�������
/// </summary>
/// <param name="plugin_name"></param>
/// <returns></returns>
const std::shared_ptr <ServerData>& ConfigFile::GetServerDataByName(const std::string& server_name)
{
	return m_server_config[server_name];
}

const std::vector<std::shared_ptr<ServerData>>& ConfigFile::GetServersByPluginName(const std::string& plugin_name)
{
	return m_plugin_config[plugin_name];
}

const std::vector<std::shared_ptr<ServerData>>& ConfigFile::GetServersByType(const std::string& server_type)
{
	return m_type_config[server_type];
}

const std::shared_ptr<ServerData>& ConfigFile::GetMyServerInfo()
{
	return m_server_config[m_server_name];
}

const std::vector<std::shared_ptr<PluginData>>& ConfigFile::GetPluginsByServerName(const std::string& server_name)
{
	return m_plugin_list[server_name];
}