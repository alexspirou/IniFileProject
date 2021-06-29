#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class COptions
{
public:
	
	COptions();
	~COptions() {}
	

	bool validate();

	const std::string m_sIniFileName{ "Application.ini" };


private:

	std::vector<std::string> m_vDefaultValues{ 200 };


};

