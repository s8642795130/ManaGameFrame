#pragma once
#include <string>
#include <vector>
#include <array>

#include "DefineHeader.h"

class HttpBuffer
{
private:
	std::vector<char> m_buffer;
	bool m_is_first_recv;
public:
	void DecteadHeader();
};