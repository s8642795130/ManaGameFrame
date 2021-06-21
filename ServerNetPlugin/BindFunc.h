#pragma once
#include "IClientNetActor.h"
#include "../ActorPlugin/ActorMsg.h"
#include "../ActorPlugin/IThreadPoolModule.h"

class IBindFunc
{
public:
    virtual void Trigger(std::shared_ptr<IThreadPoolModule> thread_pool_module, IClientNetActor& client) = 0;
};

template<typename T>
class BindFunc : public IBindFunc
{
private:
    int m_msg_id;
    std::string m_uuid;
    void(T::* m_func_ptr)(IClientNetActor&);
    void Bind(void(T::* func)(IClientNetActor&))
    {
        m_func_ptr = func;
    }
public:
    BindFunc(const int msg_id, const std::string& uuid, void(T::* func)(IClientNetActor&)) : m_msg_id(msg_id), m_uuid(uuid)
    {
        Bind(func);
    }

    virtual void Trigger(std::shared_ptr<IThreadPoolModule> thread_pool_module, IClientNetActor& client)
    {
        std::unique_ptr<IActorMsg> actor_msg = std::make_unique<ActorMsg<void, T, IClientNetActor&>>("", m_uuid, m_func_ptr, client);
        thread_pool_module->AddActorMsgToThreadCell(actor_msg);
    }
};