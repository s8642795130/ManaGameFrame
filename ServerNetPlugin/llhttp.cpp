#include <iostream>

#include "IHttpBuffer.h"
#include "llhttp.h"

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
		IHttpBuffer* p_buffer = reinterpret_cast<IHttpBuffer*>(ptr->data);
		p_buffer->SetContentLength(ptr->content_length);
	}

	return 0;
}