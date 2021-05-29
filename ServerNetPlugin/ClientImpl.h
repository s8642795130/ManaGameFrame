#pragma once
#include <memory>

#include "INetProccessModule.h"
#include "../ConfigPlugin/IConfigModule.h"

class ClientImpl
{
public:
	std::shared_ptr<IConfigModule> m_config_module;
	std::shared_ptr<INetProccessModule> m_proccess_module;
};