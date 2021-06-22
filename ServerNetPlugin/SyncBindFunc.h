#pragma once
#include "IClientNetActor.h"
#include "IServerObjModule.h"

class ISyncBindFunc
{
public:
    virtual void Trigger(std::shared_ptr<IServerObjModule> server_obj_module, IClientNetActor& client) = 0;
};

template<typename T>
class SyncBindFunc : public ISyncBindFunc
{
private:
    int m_msg_id;
    void(T::* m_func_ptr)(IClientNetActor&);
    void Bind(void(T::* func)(IClientNetActor&))
    {
        m_func_ptr = func;
    }
public:
    SyncBindFunc(const int msg_id, void(T::* func)(IClientNetActor&)) : m_msg_id(msg_id)
    {
        Bind(func);
    }

    virtual void Trigger(std::shared_ptr<T> ptr, IClientNetActor& client)
    {
        (ptr->*m_func_ptr)(client);
    }
};