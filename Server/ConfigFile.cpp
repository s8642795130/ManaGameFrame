#include "ConfigFile.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <regex>

/// <summary>
/// 分割字符串
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
/// 读取服务器配置文件
/// </summary>
/// <returns></returns>
bool ConfigFile::ReadServerConfigFile()
{
	// 文件流形式打开文件
	std::ifstream in{ m_config_file_name, std::ifstream::in };

	if (!in)
	{
		std::cerr << "error" << std::endl;
		// std::exit(1);
		return false;
	}

	// 定义文件流 开始与结束 迭代器
	auto in_iter = std::istream_iterator<std::string>(in);
	auto end_iter = std::istream_iterator<std::string>{};

	// 确定配置文件有多少行
	std::size_t size = std::distance(in_iter, end_iter);
	std::cout << "config file dis: " << size << std::endl;

	// distance 会修改迭代器位置 使用clear与seek将迭代器指向还原
	in.clear();
	in.seekg(0);

	// 将配置文件信息复制到 config_str
	std::vector<std::string> config_str{ size };
	std::copy(std::istream_iterator<std::string>(in), end_iter, std::begin(config_str));

	// 处理配置信息
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
/// 处理配置字符串
/// </summary>
/// <param name="config_str"></param>
void ConfigFile::AnalyseConfigStr(const std::vector<std::string>& config_str)
{
	// 由于是const 类型所以不可以获取迭代器 std::vector<std::string>::iterator iter = vec.begin();
	std::vector<std::string>::const_iterator c_iter = std::cbegin(config_str);
	while (c_iter != std::cend(config_str))
	{
		std::vector<std::string> param{ StrSplitBySpace(*c_iter) };
		if (param.size() == 4)
		{
			// 这是一个服务器进程配置信息
		}
		else
		{

		}
		++c_iter;
	}
}

/// <summary>
/// 根据插件名获取所有包含该类型插件的服务器组
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