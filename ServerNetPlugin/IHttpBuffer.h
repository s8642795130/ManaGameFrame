#pragma once
#include <cstdint>

class IHttpBuffer
{
public:
	virtual void SetContentLength(uint64_t content_length) = 0;
	virtual uint64_t GetContentLength() = 0;
	virtual void PushBodyStream(const char*, size_t length) = 0;
};