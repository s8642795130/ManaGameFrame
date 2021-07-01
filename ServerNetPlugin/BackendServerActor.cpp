#include "BackendServerActor.h"

void BackendServerActor::SetClientType(EnumDefine::ClientType client_type)
{
	m_client_type = client_type;
}

void BackendServerActor::ProcessIO()
{
	switch (m_client_type)
	{
	case EnumDefine::ClientType::FRONTEND:
		ProcessServerBackendIO();
		break;
	case EnumDefine::ClientType::BACKEND:
		// backend server
		ProcessRPCIO();
		break;
	default:
		break;
	}
}

void BackendServerActor::ProcessServerBackendIO()
{

}

void BackendServerActor::ProcessRPCIO()
{

}