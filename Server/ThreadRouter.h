#pragma once
#include <string>

class ThreadRouter
{
public:
	static unsigned long GetThreadIndexByUUID(unsigned int thread_num, const std::string& str_uuid)
	{
		auto str_sub = str_uuid.substr(0, 2);
		auto temp_uuid = std::strtoul(str_sub.c_str(), 0, 16);
		return temp_uuid % thread_num;
	}
};