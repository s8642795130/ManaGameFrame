#include "ClientNetControlActor.h"

void ClientNetControlActor::ProcClientMessage(ClientNet* ptr_client)
{
	ptr_client->ProccessIO();
}