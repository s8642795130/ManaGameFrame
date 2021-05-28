#pragma once
#include "IClientNetActor.h"
#include "ClientImpl.h"
#include "NetDefine.h"

class ClientNetActor : public IClientNetActor
{
private:
	std::unique_ptr<ClientImpl> m_impl;
	std::shared_ptr<ByteBuffer> m_buffer; // buffer
	// client Type
	NetDefine::ClientType m_client_type;
	const int m_timeout = 30;
protected:
	void SetClientType(NetDefine::ClientType client_type);
	void ProccessIO();
public:
	ClientNetActor();

	// interface
	virtual std::shared_ptr<ByteBuffer>& GetBuffer() override;
	virtual bool ReadReady() override;
	virtual bool WriteReady() override;
	virtual bool HeartBeat() override;
	virtual void ClientClose() override;
};

