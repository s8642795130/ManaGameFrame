#pragma once
#include <cstdint>

class IHttpBuffer
{
public:
	virtual void SetContentLength(uint64_t content_length) = 0;
};