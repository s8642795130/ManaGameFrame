#pragma once
#include <string>

class ThreadRouter
{
private:
	static unsigned int m_thread_count;
public:
	static void SetThreadCountNum(unsigned int thread_count)
	{
		m_thread_count = thread_count;
	}

	static unsigned long GetThreadIndexByUUID(const std::string& str_uuid)
	{
		auto str_sub = str_uuid.substr(0, 2);
		auto temp_uuid = std::strtoul(str_sub.c_str(), 0, 16);
		return temp_uuid % m_thread_count;
	}
};