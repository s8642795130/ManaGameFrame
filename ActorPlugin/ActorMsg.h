#pragma once

#pragma once
#include <functional>
#include <iostream>

#include "IActorMsg.h"
#include "IActor.h"

namespace helper
{
	template <std::size_t... Is>
	struct index {};

	template <std::size_t N, std::size_t... Is>
	struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

	template <std::size_t... Is>
	struct gen_seq<0, Is...> : index<Is...> {};
}

template <typename BaseClass, typename Return, typename... Params>
class ActorMsg : public IActorMsg
{
private:
	std::string m_sender_actor_uuid;
	std::string m_receiver_actor_uuid;
	std::_Mem_fn<Return(BaseClass::*)(Params...)> m_f;
	std::tuple<Params...> m_args;
	BaseClass* m_base_ptr = nullptr;
public:
	template <typename Func, typename... Args>
	ActorMsg(const std::string& sender_actor_uuid, const std::string& receiver_actor_uuid, Func f, Args&&... args) :
		m_sender_actor_uuid(sender_actor_uuid),
		m_receiver_actor_uuid(receiver_actor_uuid),
		m_f(std::mem_fn(f)),
		m_args(std::forward<Args>(args)...)
	{
	}

	void SetActorCallback()
	{
	}

	template <typename... Args, long unsigned int... Is>
	void func(std::tuple<Args...>& tup, helper::index<Is...>)
	{
		try
		{
			m_f(m_base_ptr, std::get<Is>(tup)...);
		}
		catch (...)
		{
			std::perror("m_f error: ");
		}
	}

	template <typename... Args>
	void func(std::tuple<Args...>& tup)
	{
		func(tup, helper::gen_seq<sizeof...(Args)>{});
	}

	virtual void Act(IActor* ptr)
	{
		m_base_ptr = (decltype(m_base_ptr))ptr;
		func(m_args);
	}

	virtual const std::string& GetReceiverActorUUID()
	{
		return m_receiver_actor_uuid;
	}
};

template <typename T, typename R, typename... Params, typename... Args>
std::unique_ptr<IActorMsg> CreateActorMsg(const std::string& sender_actor_uuid, const std::string& receiver_actor_uuid, R(T::* fn)(Params...), Args&&... args)
{
	return std::make_unique<ActorMsg<T, R, Args...>>(sender_actor_uuid, receiver_actor_uuid, fn, std::forward<Args>(args)...); // actor_msg(fn, args...);
}
