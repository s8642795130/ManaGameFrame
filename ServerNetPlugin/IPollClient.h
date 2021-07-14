#pragma once
#include <vector>
#include <memory>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

class ByteBuffer;

class IPollClient
{
public:
	int m_fd = 0;

	virtual std::shared_ptr<ByteBuffer> GetBuffer() = 0;
	virtual void SendData(const int major, const int minor, std::vector<char> value) = 0;
	virtual void PushData(const char* ptr_data, ssize_t length) = 0;
	virtual void ProcessIO() = 0;
};