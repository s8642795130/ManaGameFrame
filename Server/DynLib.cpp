#include <dlfcn.h>

#include "DynLib.h"

DynLib::DynLib(const std::string& name)
{
	m_lib_name = name;
}

bool DynLib::LoadLib()
{
	std::string str_lib_path = "./";
	str_lib_path += m_lib_name;
	str_lib_path += ".so";
	m_instance = dlopen(str_lib_path.c_str(), RTLD_LAZY | RTLD_GLOBAL);

	// test code
	if (!m_instance)
	{
		char* errstr;

		errstr = dlerror();
		if (errstr != NULL)
			printf("A dynamic linking error occurred: (%s)\n", errstr);
	}

	return true;
}

void* DynLib::GetSymbol(const std::string& proc_name)
{
	return dlsym(m_instance, proc_name.c_str());
}