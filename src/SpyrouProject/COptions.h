#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////// 
//--------------------------------------------------------------------------------//
// COptions
//--------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////// 
class COptions
{
public:
	
	COptions();
	~COptions() {}
	
	bool isValid(); //Validate the values and return true if are valid. If validation failed set default values.

private:

	//Default values
	std::string m_sVersion{ "2.7.8" };
	std::string m_sLogFolder{ "--" };
	std::string m_sLogFile{ "C:\\Log\LogFile.dbg" };
	std::string m_sMaxThreads{ "1" };
	std::string m_sMinCode{"-1"};
	std::string m_sMaxCode{"1"};
	std::string m_sResolution{ "100" };
	std::string m_sRetrieveType{ "DISTANCE" };
	std::string m_sCoverageMapAlgorithm{ "BASIC" };
	std::string m_sIgnoreTxWithMissingLosses{"NO"};
	std::string m_sMaxRadiusUnit{"KM"};
	std::string m_sBasicParameters{" - 106.0,100,150.0,10000.0,1,9.0,1,-98.0,2.0,-98.0,3.0,1,0,0"};
	//Ini file path
	const std::string m_sIniFileName{ "Application.ini" };
};

