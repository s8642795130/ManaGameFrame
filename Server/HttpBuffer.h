#pragma once
#include <string>
#include <vector>
#include <array>

#include "DefineHeader.h"
#include "llhttp.h"

int handle_on_message_complete(llhttp_t* ptr)
{
	std::cout << "handle_on_message_complete" << std::endl;
	std::cout << ptr->header_state << std::endl;
	std::cout << "content_length: " << ptr->content_length << std::endl;
	std::cout << "status_code: " << ptr->status_code << std::endl;
	// char* str = (char*)ptr->data;
	// std::cout << strlen(str) << std::endl;

	return 0;
}

int on_body(llhttp_t* ptr, const char* at, size_t length)
{
	std::cout << "length: " << length << std::endl;
	std::cout << "at: " << at << std::endl;
	return 0;
}

int on_header_value_complete(llhttp_t* ptr)
{
	std::cout << "on_header_value_complete" << std::endl;
	std::cout << ptr->header_state << std::endl;
	if (ptr->flags & F_CONTENT_LENGTH)
	{
		std::cout << "content_length: " << ptr->content_length << std::endl;
	}
}

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
		llhttp_t parser;
		llhttp_settings_t settings;

		/* Initialize user callbacks and settings */
		llhttp_settings_init(&settings);

		/* Set user callback */
		settings.on_message_complete = handle_on_message_complete;
		settings.on_body = on_body;
		settings.on_header_value_complete = on_header_value_complete;

		enum llhttp_errno err = llhttp_execute(&parser, buffer.data(), buffer.size());
		if (err == HPE_OK)
		{
			
			/* Successfully parsed! */
			std::cout << "Successfully parsed!" << parser.content_length << std::endl;
		}
		else
		{
			fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err), parser.reason);
			std::cout << "Error" << std::endl;
		}

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