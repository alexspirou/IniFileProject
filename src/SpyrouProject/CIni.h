#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include "CIni.h"
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////// 
//--------------------------------------------------------------------------------//
// CIni
//--------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////// 
class CIni
{


public:
	//Constructor/Destructor
	CIni();
	~CIni() { writeLogFile("Program has stopped", 1, 1); }
	//File operations
	void writeLogFile(std::string msg, bool bTime, bool startsEnd);				//Write the events to logfile
	size_t readIniFile();														//Read the file and return the count of headers
	void writeIniFile(bool userInput);											//Write to .ini file 
	void createLogFile();														//Creates a log folder and file if it doesn't exist


private:
	//Filename 
	const std::string m_sIniFileName{ "Application.ini" };
	//Log file name
	const std::string m_sLogFileName{ "Logfile.dbg" };
	//Path for log folder 
	const std::string m_sLogFilePath{ "C:\\Application_Log" };

};
