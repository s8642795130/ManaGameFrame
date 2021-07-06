#pragma once
#include <string>
#include <vector>
#include <array>
#include <cstring>

#include "DefineHeader.h"

class ByteBuffer
{
private:
	int m_buf_length = 0;
	std::vector<char> m_buf_ptr;
	std::array<char, HEADER_LENGTH> m_msg_header;
	std::array<char, 4> m_length_buf;
	int m_cur_pos = 0;
	ssize_t m_remaining_len = HEADER_LENGTH; // majorId: 4 minorId: 4 dataLength: 4
	bool m_is_received_header = false;
	//
	int m_major_id = 0;
	int m_minor_id = 0;
public:
	ByteBuffer() = default;
	virtual ~ByteBuffer() = default;

	int GetMajorId()
	{
		return m_major_id;
	}

	int GetMinorId()
	{
		return m_minor_id;
	}

	int GetSize()
	{
		return m_buf_length;
	}

	bool GetHeaderStatus()
	{
		return m_is_received_header;
	}

	int GetCurPos()
	{
		return m_cur_pos;
	}

	void StepBufferPos(int step)
	{
		m_cur_pos += step;
	}

	bool ResetHeader()
	{
		std::memcpy(&m_major_id, m_msg_header.data() , MAJOR_LENGTH); // recv major
		std::memcpy(&m_minor_id, m_msg_header.data() + MAJOR_LENGTH, MINOR_LENGTH); // recv minor
		std::memcpy(&m_buf_length, m_msg_header.data() + MAJOR_LENGTH + MINOR_LENGTH, INT_LENGTH); // recv length

		// check data is valid
		if (m_major_id < 0 || m_minor_id < 0 || m_buf_length < 0)
		{
			return false;
		}

		// less length
		m_remaining_len = m_buf_length;

		// alloc memory
		if (m_buf_length != 0)
		{
			m_buf_ptr.reserve(m_buf_length);
		}

		// set header flag
		m_is_received_header = true;
		return true;
	}

	void ResetData()
	{
		m_buf_length = 0;
		m_cur_pos = 0;
		m_remaining_len = HEADER_LENGTH;
		m_msg_header[HEADER_LENGTH] = { 0 };
		m_is_received_header = false;
	}

	ssize_t GetRemainingLen()
	{
		return m_remaining_len;
	}

	void RecvHeader(char* buf, std::size_t len)
	{
		std::memcpy(m_msg_header.data() + (HEADER_LENGTH - m_remaining_len), buf, len);
		m_remaining_len -= len;
	}

	void PushData(char* buf, std::size_t len)
	{
		std::memcpy(m_buf_ptr.data(), buf, len);
		m_remaining_len -= len;
	}

	std::vector<char> GetStream()
	{
		return m_buf_ptr;
	}

	// data

	int GetInt()
	{
		int value = 0;
		std::memcpy(&value, m_buf_ptr.data() + m_cur_pos, 4);
		m_cur_pos += 4;
		return value;
	}

	bool GetBool()
	{
		bool value = false;
		std::memcpy(&value, m_buf_ptr.data() + m_cur_pos, 1);
		m_cur_pos += 1;
		return false;
	}

	char GetChar()
	{
		char value = 0;
		std::memcpy(&value, m_buf_ptr.data() + m_cur_pos, 1);
		m_cur_pos += 1;
		return value;
	}

	std::string GetString(int length)
	{
		// copy buffer
		std::vector<char> value(length + 1);
		std::strncpy(value.data(), reinterpret_cast<const char*>(m_buf_ptr.data()) + m_cur_pos, length);
		value[length] = '\0';
		
		//
		m_cur_pos += length;

		// return value
		std::string str(value.data());
		return str;
	}
};
