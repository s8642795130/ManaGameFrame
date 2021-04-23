#pragma once
#include <string>

#include "DefineHeader.h"

class ByteBuffer
{
private:
	int m_buf_length = 0;
	char* m_buf_ptr = nullptr;
	char m_msg_header[HEADER_LENGTH] = { 0 };
	char m_length_buf[4] = { 0 };
	std::size_t m_len_recv_num = 0;
	int m_cur_pos = 0;
	std::size_t m_remain_len = HEADER_LENGTH; // majorId: 4 minorId: 4 dataLength: 4
	bool m_is_received_header = false;
	//
	int m_major_id = 0;
	int m_minor_id = 0;
public:
	ByteBuffer() = default;

	virtual ~ByteBuffer()
	{
		if (m_buf_ptr != nullptr)
		{
			delete[] m_buf_ptr;
		}
	}

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

	void ResetHeader()
	{
		std::cout << "recv header over" << std::endl;

		std::memcpy(&m_major_id, m_msg_header, MAJOR_LENGTH); // recv major
		std::memcpy(&m_minor_id, m_msg_header + MAJOR_LENGTH, MINOR_LENGTH); // recv minor
		std::memcpy(&m_buf_length, m_msg_header + MAJOR_LENGTH + MINOR_LENGTH, INT_LENGTH); // recv length

		// set recv length
		m_len_recv_num = m_buf_length;

		// less length
		m_remain_len = m_buf_length;

		// alloc memory
		if (m_buf_length != 0)
		{
			m_buf_ptr = new char[m_buf_length];
		}

		// set header flag
		m_is_received_header = true;
	}

	void ResetData()
	{
		m_buf_length = 0;
		delete[] m_buf_ptr;
		m_buf_ptr = nullptr;
		m_len_recv_num = 0;
		m_cur_pos = 0;
		m_remain_len = HEADER_LENGTH;
		m_msg_header[HEADER_LENGTH] = { 0 };
		m_is_received_header = false;
	}

	std::size_t GetRemainLen()
	{
		return m_remain_len;
	}

	void RecvHeader(char* buf, std::size_t len)
	{
		std::memcpy(m_msg_header + (HEADER_LENGTH - m_remain_len), buf, len);
		m_remain_len -= len;
	}

	std::size_t GetUnreceivedLen()
	{
		return m_len_recv_num;
	}

	void PushData(char* buf, std::size_t len)
	{
		std::memcpy(m_buf_ptr, buf, len);
		m_len_recv_num -= len;
	}

	// data

	int GetInt()
	{
		int value = 0;
		std::memcpy(&value, m_buf_ptr + m_cur_pos, 4);
		m_cur_pos += 4;
		return value;
	}

	bool GetBool()
	{
		return false;
	}

	char GetChar()
	{
		char value = 0;
		std::memcpy(&value, m_buf_ptr + m_cur_pos, 1);
		m_cur_pos += 1;
		return value;
	}

	std::string GetString(int length)
	{
		return "";
	}
};
