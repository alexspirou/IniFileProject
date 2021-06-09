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
	void readFile();															//Read the file and store the values
	void writeFile(bool userInput, std::vector<std::string> m_vInputs);			//Write to .ini file 
	bool validateFile();														//Validate the file returns true if it's validated

	//Getters

	inline std::vector<std::string>& getHeaderValues() { return m_vHeaderValues; }		//Getter for program values vector
	inline std::vector<std::string>& getDefaultValues() { return m_vDefaultValues; }	//Getter for default values string				
	inline int getErrorHandler() { return m_iErrorHandler; }							//Getter for error handler
	//Setters
	inline void setErrorHandler(int newError) { m_iErrorHandler = newError; }

private:

	//File operations
	void createLogFile();																//Creates a log folder and file if it doesn't exist

	//File in/out objects
	std::ifstream inFile{};
	std::ofstream outFile{};
	//Filename 
	const std::string m_sIniFileName{ "Application.ini" };
	//Log file name
	const std::string m_sLogFileName{ "Logfile.dbg" };
	//Path for log folder 
	const std::string m_sLogFilePath{ "C:\\Application_Log" };
	//Full path for log file
	const std::string m_sLogFileFullPath = m_sLogFilePath + "\\" + m_sLogFileName;

	//Section Names for special use
	std::string m_sFirstHeaderName = { "[ -GENERAL SECTION- ]" };
	std::vector<std::string> m_vSectionNames{ "[ -A SECTION- ]" , "[ -B SECTION- ]" };
	std::vector<std::string> m_vInputIntOnly{ "[ MAX THREADS ]","[ MIN CODE ]","[ MAX CODE ]","[ RESOLUTION ]" };
	std::string m_sVersionHeader = { "[ VERSION ]" };

	//Next string for iterate the file
	std::string m_sNextLine{};

	//Vectors 

	//Vector for storing Headers
	std::vector<std::string> m_vHeaders;
	//Vector for storing Values
	std::vector<std::string> m_vHeaderValues;
	//Vector for storing Validation Data
	std::vector<std::string> m_vValData;
	//Vector for default values
	std::vector<std::string> m_vDefaultValues;
	size_t m_szHeaderSize{ 0 };

	//integer to store the section name indices
	int m_iIndexSectionA{ 0 };
	int m_iIndexSectionB{ 0 };

	int m_iErrorHandler = 0;

};
