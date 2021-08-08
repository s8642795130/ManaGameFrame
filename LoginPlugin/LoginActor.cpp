#include "LoginActor.h"
#include "../ServerNetPlugin/BindFunc.h"
#include "../ServerNetPlugin/FrontendHttpActor.h"

void LoginActor::OnLoginOnlineCallback(std::shared_ptr<INetActor> ptr_client, std::string uuid, std::string path, std::string query)
{
	std::cout << __func__ << std::endl;

	std::vector<char> buffer;
	buffer.push_back('t');
	buffer.push_back('e');
	buffer.push_back('s');
	buffer.push_back('t');

	std::unique_ptr<IActorMsg> ptr = CreateActorMsg(GetUUID(), uuid, &FrontendHttpActor::SendStream, std::move(buffer));
	m_pimpl->SendMsgToActor(ptr);
}