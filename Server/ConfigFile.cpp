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
std::vector<std::string> ConfigFile::StrSplitBySpace(const std::string& s)
{
	std::string text = s;
	std::regex ws_re("\\s+"); // whitespace
	std::vector<std::string> v(std::sregex_token_iterator(text.begin(), text.end(), ws_re, -1),
		std::sregex_token_iterator());

	return v;
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
		std::cerr << "error" << std::endl;
		// std::exit(1);
		return false;
	}

	// �����ļ��� ��ʼ����� ������
	auto in_iter = std::istream_iterator<std::string>(in);
	auto end_iter = std::istream_iterator<std::string>{};

	// ȷ�������ļ��ж�����
	std::size_t size = std::distance(in_iter, end_iter);
	std::cout << "config file dis: " << size << std::endl;

	// distance ���޸ĵ�����λ�� ʹ��clear��seek��������ָ��ԭ
	in.clear();
	in.seekg(0);

	// �������ļ���Ϣ���Ƶ� config_str
	std::vector<std::string> config_str{ size };
	std::copy(std::istream_iterator<std::string>(in), end_iter, std::begin(config_str));

	// ����������Ϣ
	AnalyseConfigStr(config_str);

	return true;
}

/// <summary>
/// ���������ַ���
/// </summary>
/// <param name="config_str"></param>
void ConfigFile::AnalyseConfigStr(const std::vector<std::string>& config_str)
{
	// ������const �������Բ����Ի�ȡ������ std::vector<std::string>::iterator iter = vec.begin();
	std::vector<std::string>::const_iterator c_iter = std::cbegin(config_str);
	while (c_iter != std::cend(config_str))
	{
		std::vector<std::string> param{ StrSplitBySpace(*c_iter) };
		if (param.size() == 4)
		{
			// ����һ������������������Ϣ
			std::shared_ptr<ServerData> server_data = std::make_shared<ServerData>();

			//
			m_server_config[server_data->m_server_name] = server_data;

			//
			std::vector<std::shared_ptr<PluginData>> vec_plugin_data;
			m_plugin_list[server_data->m_server_name] = vec_plugin_data;

			//
			std::vector<std::shared_ptr<ServerData>> vec_server_list;
			vec_server_list.push_back(server_data);
			m_type_config[param[0]] = vec_server_list;
		}
		else
		{
			// m_plugin_list[param[1]].push_back();
		}
		++c_iter;
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