#pragma once
#include <string>

class ByteBuffer
{
private:
	int m_buf_length = 0;
	char* m_buf_ptr = nullptr;
	char m_length_buf[4] = { 0 };
	int m_len_recv_num = 0;
	int m_cur_pos = 0;
	int m_remain_len = 4;
public:
	ByteBuffer() = default;

	virtual ~ByteBuffer()
	{
		delete[] m_buf_ptr;
	}

	int GetSize()
	{
		return m_buf_length;
	}

	void ResetSize()
	{
		std::memcpy(&m_buf_length, m_length_buf, 4);
		m_buf_ptr = new char[m_buf_length];
		m_len_recv_num = m_buf_length;
	}

	void ResetData()
	{
		m_buf_length = 0;
		delete[] m_buf_ptr;
		m_buf_ptr = nullptr;
		m_len_recv_num = 0;
		m_cur_pos = 0;
		m_remain_len = 4;
	}

	int GetRemainLen()
	{
		return m_remain_len;
	}

	void RecvLength(char* buf, int len)
	{
		std::memcpy(m_length_buf + (4 - m_remain_len), buf, len);
		m_remain_len -= len;
	}

	int GetUnreceivedLen()
	{
		return m_len_recv_num;
	}

	void PushData(char* buf, int len)
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
