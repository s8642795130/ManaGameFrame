#pragma once
#include "IPollClient.h"
#include "ClientPimpl.h"
#include "../Server/ByteBuffer.h"

class PollClient : public IPollClient
{
protected:
	// impl
	std::shared_ptr<ClientPimpl> m_client_impl;
	// buffer
	std::shared_ptr<ByteBuffer> m_buffer;
public:
	PollClient(std::shared_ptr<ClientPimpl> ptr_impl) : m_client_impl(ptr_impl), m_buffer(std::make_shared<ByteBuffer>())
	{
	}

	virtual std::shared_ptr<ByteBuffer> GetBuffer() override;
	virtual void SendData(const int major, const int minor, std::vector<char> value) override;
	virtual void PushData(const char* ptr_data, ssize_t length) override;
	virtual void ProcessIO() override;
};

