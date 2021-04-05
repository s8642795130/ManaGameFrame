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
	/*
	std::for_each(std::begin(config_str), std::end(config_str), [](auto& value) -> void
		{
			std::cout << value << std::endl;
		}
	);
	*/

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
		}
		else
		{

		}
		++c_iter;
	}
}

/// <summary>
/// ���ݲ������ȡ���а��������Ͳ���ķ�������
/// </summary>
/// <param name="plugin_name"></param>
/// <returns></returns>
std::vector<int> ConfigFile::GetServersByPluginName(const std::string& plugin_name)
{
	std::vector<int> vec;
	return vec;
}

std::vector<int> ConfigFile::GetPluginsByPluginName(const std::string& plugin_name)
{
	std::vector<int> vec;
	return vec;
}