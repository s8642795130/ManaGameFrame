#pragma once
#include <string>
#include <vector>
#include <array>

#include "DefineHeader.h"
#include "ByteBuffer.h"
#include "llhttp.h"

int handle_on_message_complete(llhttp_t* ptr);
int on_body(llhttp_t* ptr, const char* at, size_t length);
int on_header_value_complete(llhttp_t* ptr);

class HttpBuffer
{
private:
	std::vector<char> m_buffer;
	bool m_is_first_recv = true;
	int m_data_state = -1;
	int m_remaining_len = 0;
	//
	uint64_t m_content_length = 0;
	bool m_b_content_length = false;
	bool m_ready_to_execute = false;
	int m_more_data_length = 0;
	// buffer
	int m_body_pos = 0;
	std::shared_ptr<ByteBuffer> m_byte_buffer;
	std::vector<char> m_stream;
protected:
	//
	bool CompareVec(std::vector<char> vec1, std::vector<char> vec2, std::size_t pos)
	{
		bool ret = true;
		for (std::size_t i = 0; i < vec2.size(); ++i)
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
		auto for_times = vec1.size() - vec2.size();
		int pos = -1;
		if (for_times > 0)
		{
			for (std::size_t i = 0; i < for_times; ++i)
			{
				if (CompareVec(vec1, vec2, i))
				{
					pos = static_cast<int>(i);
					break;
				}
			}
		}

		return pos;
	}

	bool ParsingHeader(std::vector<char> buffer)
	{
		llhttp_t parser;
		llhttp_settings_t settings;

		// Initialize user callbacks and settings
		llhttp_settings_init(&settings);

		// Set user callback
		settings.on_message_complete = handle_on_message_complete;
		settings.on_body = on_body;
		settings.on_header_value_complete = on_header_value_complete;

		// set data
		parser.data = this;

		enum llhttp_errno err = llhttp_execute(&parser, buffer.data(), buffer.size());
		if (err == HPE_OK)
		{
			// Successfully parsed
			std::cout << "Successfully parsed!" << parser.content_length << std::endl;
		}
		else
		{
			fprintf(stderr, "Parse error: %s %s\n", llhttp_errno_name(err), parser.reason);
		}
	}

	bool ParsingBodyToBuffer()
	{
		if (m_stream.size() < HEADER_LENGTH)
		{
			return false;
		}

		m_byte_buffer->PushData(m_stream.data(), HEADER_LENGTH);
		m_byte_buffer->ResetHeader();
		m_byte_buffer->PushData(m_stream.data() + HEADER_LENGTH, m_stream.size() - HEADER_LENGTH);

		if (m_byte_buffer->GetRemainingLen() == 0)
		{
			return true;
		}

		return false;
	}
public:
	HttpBuffer() : m_byte_buffer(std::make_shared<ByteBuffer>())
	{

	}

	int DetectHeader()
	{
		auto ret = -1;
		auto temp_vec = std::vector{ '\r', '\n', '\r', '\n' };
		auto pos = FindVecFromVec(m_buffer, temp_vec);
		if (pos > 0)
		{
			// alread find "\r\n"
			m_body_pos = pos + 3;

			int cur_body_len = static_cast<int>(m_buffer.size() - (pos + 4));

			// parsing header
			if (ParsingHeader(m_buffer))
			{
				// 3 condition
				if (m_content_length == cur_body_len)
				{
					//
					ParsingBodyToBuffer();
					m_ready_to_execute = true;
					m_data_state = 0;
					ret = 0;
				}

				if (m_content_length < cur_body_len) // sticky package
				{
					// more data
					ParsingBodyToBuffer();
					m_ready_to_execute = true;
					m_more_data_length = cur_body_len - m_content_length;
					m_data_state = 2;
					ret = 2;
				}

				if (m_content_length > cur_body_len) // half package
				{
					// need more data
					m_remaining_len = m_content_length - cur_body_len;
					m_data_state = 1;
					ret = 1;
				}
			}
		}
		else
		{
			// can not find http header, need more data
			m_data_state = -1;
			ret = -1;
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

	const int GetMoreDataLength()
	{
		return m_more_data_length;
	}

	void PushData(char* data, int length)
	{
		for (auto i = 0; i < length; ++i)
		{
			m_buffer.push_back(data[i]);
		}
	}

	void SetContentLength(uint64_t content_length)
	{
		m_b_content_length = true;
		m_content_length = content_length;
	}

	bool ReadyToExecute()
	{
		return m_ready_to_execute;
	}

	void Reset()
	{
		m_is_first_recv = true;
		m_data_state = -1;
		m_remaining_len = 0;
		//
		m_content_length = 0;
		m_b_content_length = false;
		m_ready_to_execute = false;
		m_more_data_length = 0;
		m_body_pos = 0;
	}

	// interface
	std::shared_ptr<ByteBuffer>& GetBuffer()
	{
		return m_byte_buffer;
	}
};

// Parsing function

int handle_on_message_complete(llhttp_t* ptr)
{
	return 0;
}

int on_body(llhttp_t* ptr, const char* at, size_t length)
{
	// std::cout << "length: " << length << std::endl;
	// std::cout << "at: " << at << std::endl;
	return 0;
}

int on_header_value_complete(llhttp_t* ptr)
{
	if (ptr->flags & F_CONTENT_LENGTH)
	{
		std::cout << "content_length: " << ptr->content_length << std::endl;
		HttpBuffer* p_buffer = reinterpret_cast<HttpBuffer*>(ptr->data);
		p_buffer->SetContentLength(ptr->content_length);
	}

	return 0;
}