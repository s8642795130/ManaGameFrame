#pragma once
#include <memory>
#include <map>
#include <string>
#include <iostream>
#include <cassert>

class IPluginManager;
class IModule;

class IPlugin
{
protected:
	std::shared_ptr<IPluginManager> m_ptr_manager;
public:
	/// <summary>
	/// IPlugin
	/// </summary>
	/// <param name="app"></param>
	IPlugin(std::shared_ptr<IPluginManager> ptr)
	{
		m_ptr_manager = ptr;
	}

	//
	virtual void Install() = 0;
	virtual void Uninstall() = 0;
};
