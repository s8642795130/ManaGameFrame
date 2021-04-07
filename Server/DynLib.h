#pragma once
#include <string>

class DynLib
{
private:
	std::string m_lib_name;
	void* m_instance = nullptr;
public:
	DynLib(const std::string& name);
	virtual ~DynLib() = default;
	//
	bool LoadLib();
};

