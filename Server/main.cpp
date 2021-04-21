#include <iostream>
#include <cstring>
#include <functional>
#include <memory>

#include "ConfigFile.h"
#include "Application.h"

#include "ActorMsg.h"
#include "Actor.h"

class A : public Actor
{
public:
	A() : Actor()
	{

	}

	virtual ~A()
	{

	}

	void SendMsg(const int a, const int b, const char* p, const int length)
	{
		std::cout << a << " " << b << " " << p << " " << length << std::endl;
	}
};

std::shared_ptr<IApplication> IApplication::m_app;

/*
namespace helper
{
    template <std::size_t... Ts>
    struct index {};

    template <std::size_t N, std::size_t... Ts>
    struct gen_seq : gen_seq<N - 1, N - 1, Ts...> {};

    template <std::size_t... Ts>
    struct gen_seq<0, Ts...> : index<Ts...> {};
}

template <typename... Ts>
class Action_impl
{
private:
    std::function<void(Ts...)> f;
    std::tuple<Ts...> args;
public:
    template <typename F>
    Action_impl(F&& func, Ts&&... args)
        : f(std::forward<F>(func)),
        args(std::make_tuple(std::forward<Ts>(args)...))
    {}

    template <typename... Args, std::size_t... Is>
    void func(A* ptr, std::tuple<Args...>& tup, helper::index<Is...>)
    {
        f(std::get<Is>(tup)...);
    }

    template <typename... Args>
    void func(A* ptr, std::tuple<Args...>& tup)
    {
        func(tup, helper::gen_seq<sizeof...(Args)>{});
    }

    void act(A* ptr)
    {
        func(args);
    }
};

template <typename F, typename... Args>
Action_impl<Args...> make_action(F&& f, Args&&... args)
{
    return Action_impl<Args...>(std::forward<F>(f), std::forward<Args>(args)...);
}

int main()
{
    // auto add = make_action([](int a, int b) { std::cout << a + b; }, 2, 3);
    A a;
    auto add = make_action(std::bind(&A::SendMsg, &a, 10, 20, "30", 40));

    add.act(&a);
}
*/

int main(int argc, char* argv[])
{
	// dladdr, dlclose, dlerror, dlopen, dlsym, dlvsym
	// va_list, va_start, va_arg, va_end
	std::cout << "test !!!" << std::endl;

	// Application
    /*
	std::shared_ptr<Application> app{ std::make_shared<Application>() };
	app->LoadConfig();
	app->StartLoadAllLibrary();
	app->StartThreadPool();
	app->StartNetwork();
	app->ConnectMaster();
    */

	A a;
	std::unique_ptr<IActorMsg> ptr{ std::make_unique<ActorMsg<void, A, const int, const int, const char*, const int>>("send", "recv", &A::SendMsg, 10, 20, "40", 30) };
	ptr->Act(&a);

	return 0;
}