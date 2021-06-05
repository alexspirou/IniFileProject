#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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
	~CIni()
	{


	}
	void writeLogFile(std::string msg);
;	void writeFile(std::string* userInput, std::vector<std::string> m_vInputs);
	bool validateFile();
	inline std::vector<std::string>& getHeaderValues() { return m_vHeaderValues; }
	inline std::string getValidatedMsg() { return m_sValidatedMsg; }
	inline std::string getNotValidatedMsg() { return m_sNotValidatedMsg; }
private:
	void readFile();
	void createLogFile();
	
	

	std::ifstream inFile{};
	std::ofstream outFile{};
	const std::string m_sIniFileName{ "Application.ini" };
	const std::string m_sLogFileName{ "Logfile.dbg" };
	const std::string m_sLogFilePath{ "C:\\Application_Log" };
	const std::string m_sLogFileFullPath = m_sLogFilePath + "\\" + m_sLogFileName;

	std::vector<std::string> m_vSectionNames{ "[ -A SECTION- ]" , "[ -B SECTION- ]" };
	size_t headerSize{ 0 };
	std::string m_sNextLine{};

	std::vector<std::string> m_vHeaders;
	std::vector<std::string> m_vHeaderValues;
	std::vector<std::string> m_vValData;

	int indexSectionA{ 0 };
	int indexSectionB{ 0 };

	std::string m_sReadMsg{ m_sIniFileName + "  was read             " };
	std::string m_sWriteMsg{ m_sIniFileName + "  was written          " };
	std::string m_sValidatedMsg{ "Validation for " + m_sIniFileName + " succeed" };
	std::string m_sNotValidatedMsg{ "Validation for " + m_sIniFileName + " failed " };









};
