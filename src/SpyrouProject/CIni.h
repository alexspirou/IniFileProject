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
	~CIni() {}


private:


	void writeFile(std::string* userInput);

	void readFile();

	void validateFile();

	std::ifstream inFile{};
	std::ofstream outFile{};
	std::string sFileName{ "Application.ini" };
	std::vector<std::string> m_vSectionNames{ "[ -A SECTION- ]" , "[ -B SECTION- ]" };
	size_t headerSize{ 0 };
	std::string m_sNextLine{};

	std::vector<std::string> m_vHeaders;
	std::vector<std::string> vHeaderValues;
	std::vector<std::string> m_vValData;

	int indexSectionA{ 0 };
	int indexSectionB{ 0 };

};

