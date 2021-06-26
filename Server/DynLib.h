#pragma once
#include <string>

class DynLib
{
private:
	std::string m_lib_path_name;
	void* m_instance = nullptr;
public:
	DynLib(const std::string& name);
	virtual ~DynLib() = default;
	//
	bool LoadLib();
	void* GetSymbol(const std::string& proc_name);
};

