#pragma once
#include "IClientNetActor.h"
#include "ClientPimpl.h"

#include "../Server/ServerEnumDefine.h"

class ClientNetActor : public IClientNetActor
{
protected:
	// client data
	std::shared_ptr<ClientPimpl> m_client_impl;
	std::shared_ptr<ByteBuffer> m_buffer; // buffer
	std::map<std::string, std::string> m_client_data; // team id, room id ...

	// client Type
	std::string m_uid;
	EnumDefine::ClientType m_client_type;

	// timeout
	const int m_timeout = 30;
	time_t m_last_active;
protected:
	void Parsing(std::array<char, DEFAULT_BUFLEN>& buffer, ssize_t len);
	void ProccessIO();
public:
	ClientNetActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr);

	// interface
	virtual std::shared_ptr<ByteBuffer>& GetBuffer() override;
	virtual bool ConnectServer(const std::string& ip, const int port);
	virtual bool ReadReady() override;
	virtual bool WriteReady() override;
	virtual bool HeartBeat() override;
	virtual void ClientClose() override;
	//
	virtual void SetClientType(EnumDefine::ClientType client_type);
	// client data
	virtual const std::map<std::string, std::string> GetClientData() const;
	virtual void UpdateClientData(const std::string& key, const std::string& value, int type);
	//
	virtual void SetUid(const std::string& uid);
	virtual const std::string& GetUid() const;
	virtual void SendData(const int major, const int minor, std::vector<char> value);
	// virtual void SendBuffer(std::shared_ptr<ByteBuffer> buffer);
	virtual void SendStream(std::vector<char> stream);
};

