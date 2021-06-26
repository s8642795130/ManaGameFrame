#include <dlfcn.h>
#include <iostream>

#include "DynLib.h"

DynLib::DynLib(const std::string& path)
{
	m_lib_path_name = path;
}

bool DynLib::LoadLib()
{
	m_instance = dlopen(m_lib_path_name.c_str(), RTLD_LAZY | RTLD_GLOBAL);

	// test code
	if (!m_instance)
	{
		auto errstr = dlerror();
		if (errstr != nullptr)
			std::cerr << "A dynamic linking error occurred: " << errstr << std::endl;
	}

	return true;
}

void* DynLib::GetSymbol(const std::string& proc_name)
{
	return dlsym(m_instance, proc_name.c_str());
}