#pragma once
#include <string>
#include <vector>
#include <array>

#include "DefineHeader.h"

class HttpBuffer
{
private:
	std::vector<char> m_buffer;
	bool m_is_first_recv = true;
	int m_data_state = -1;
	int m_remaining_len = 0;
protected:
	//
	bool CompareVec(std::vector<char> vec1, std::vector<char> vec2, int pos)
	{
		bool ret = true;
		for (auto i = 0; i < vec2.size(); ++i)
		{
			if (vec1[pos] != vec2[i])
			{
				ret = false;
				break;
			}
		}

		return ret;
	}

	// find "\r\n\r\n"
	int FindVecFromVec(std::vector<char> vec1, std::vector<char> vec2)
	{
		int ret = -1;
		auto for_times = vec1.size() - vec2.size();
		int pos = -1;
		if (for_times > 0)
		{
			for (auto i = 0; i < for_times; ++i)
			{
				if (CompareVec(vec1, vec2, i))
				{
					pos = i;
					break;
				}
			}
		}

		return pos;
	}

	int ParsingHeader(std::vector<char> buffer)
	{
		return 10;
	}
public:
	int DetectHeader()
	{
		auto ret = -1;
		auto temp_vec = std::vector{ '\r', '\n', '\r', '\n' };
		auto pos = FindVecFromVec(m_buffer, temp_vec);
		if (pos > 0)
		{
			// alread find "\r\n"
			int cur_body_len = m_buffer.size() - (pos + 4);

			// parsing header
			int body_len = ParsingHeader(m_buffer);

			// 3 condition
			if (body_len == cur_body_len)
			{
				m_data_state = 0;
				ret = 0;
			}

			if (body_len < cur_body_len) // sticky package
			{
				// more data
				ret = 2;
			}

			if (body_len > cur_body_len) // half package
			{
				// need more data
				m_remaining_len = body_len - cur_body_len;
				ret = 1;
			}
		}
		else
		{
			// can not find http header, need more data
			ret = 1;
		}

		return ret;
	}

	const int GetRemainingLen()
	{
		return m_remaining_len;
	}

	const int GetDataState()
	{
		return m_data_state;
	}

	void PushData(char* data, int length)
	{
		for (auto i = 0; i < length; ++i)
		{
			m_buffer.push_back(data[i]);
		}
	}
};