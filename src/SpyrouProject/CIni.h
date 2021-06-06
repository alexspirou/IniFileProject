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
	~CIni() {  }
	//File operations
	void writeLogFile(std::string msg, bool bTime);										//Write the events to logfile
	void readFile();																	//Read the file and store the values
	void writeFile(std::string* userInput, std::vector<std::string> m_vInputs);			//Write to .ini file 
	bool validateFile();																//Validate the file returns true if it's validated

	//Getters

	inline std::vector<std::string>& getHeaderValues() { return m_vHeaderValues; }		//Getter for program values vector
	inline std::vector<std::string>& getDefaultValues() { return m_vDefaultValues; }	//Getter for default values string
	inline std::string getValidatedMsg() { return m_sValidatedMsg; }					//Getter for validate success message 
	inline std::string getNotValidatedMsg() { return m_sNotValidatedMsg; }				//Getter for validate failed message
	inline int getErrorHandler() { return m_iErrorHandler; }							//Getter for error handler

private:

	//File operations
	void createLogFile();																//Creates a log folder and file if it doesn't exist

	std::ifstream inFile{};
	std::ofstream outFile{};
	const std::string m_sIniFileName{ "Application.ini" };
	const std::string m_sLogFileName{ "Logfile.dbg" };
	const std::string m_sLogFilePath{ "C:\\Application_Log" };
	const std::string m_sLogFileFullPath = m_sLogFilePath + "\\" + m_sLogFileName;

	//Section Names
	std::string m_sFirstHeaderName = { "[ -GENERAL SECTION- ]" };
	std::vector<std::string> m_vSectionNames{"[ -A SECTION- ]" , "[ -B SECTION- ]" };
	std::vector<std::string> m_vInputIntOnly{ "[ MAX THREADS ]","[ MIN CODE ]","[ MAX CODE ]","[ RESOLUTION ]" };
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

	//
	int m_iIndexSectionA{ 0 };
	int m_iIndexSectionB{ 0 };

	//Logfile messages
	std::string m_sReadMsg{ m_sIniFileName + " read" };
	std::string m_sWriteMsg{ m_sIniFileName + " write" };
	std::string m_sValidatedMsg{ m_sIniFileName + "  validation succeed" };
	std::string m_sNotValidatedMsg{ m_sIniFileName + " validation failed" };

	int m_iErrorHandler = 0;







};
