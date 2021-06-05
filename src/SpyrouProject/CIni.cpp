#define _CRT_SECURE_NO_WARNINGS
#include "CIni.h"
#include <algorithm>
#include <ctime>
#include <windows.h>
#include <iomanip>

//TO DO FOR TOMORROW
//DEFAULT VALUES, VALID VERSION, VALID BASIC PARAMWETERS, TRY CATCH ERRORS, USER INPUT IF ITS POSSIBLE, MAKE CODE LOOK BETTER
CIni::CIni() 
{  
    createLogFile();
    readFile();
    m_vHeaderValues.resize(m_vHeaders.size() - m_vSectionNames.size());

    //Values for each section
    m_vHeaderValues[0] = "2.7.8"; // [VERSION]
    m_vHeaderValues[1] = m_sLogFilePath;    //[LOG FOLDER]
    m_vHeaderValues[2] = m_sLogFilePath + "\\" + m_sLogFileName;  //[LOG FILE]
    m_vHeaderValues[3] = "1";      //[MAX THREADS]
    m_vHeaderValues[4] = "-1"; //[MIN CODE]
    m_vHeaderValues[5] = "-1"; //[MAX CODE]
    m_vHeaderValues[6] = "100"; //[RESOLUTION]
    m_vHeaderValues[7] = "DISTANCE"; //[RETRIEVE TYPE]
    m_vHeaderValues[8] = "BASIC"; //[COVERAGE MAP ALGORITHM]
    m_vHeaderValues[9] = "YES"; //[IGNORE TX WITH MISSING LOSSES]
    m_vHeaderValues[10] = "km"; //[MAX RADIUS UNIT]
    m_vHeaderValues[11] = "-106.0,100,150.0,10000.0,1,9.0,1,-98.0,2.0,-98.0,3.0,1,0,0"; //[BASIC PARAMETERS]
    //std::string* s = new std::string();
    //s = NULL;
    //writeFile(s, m_vHeaderValues);
    //if (validateFile())
    //{
    //    writeLogFile(m_sValidatedMsg);

    //}
    //else
    //{
    //    writeLogFile(m_sNotValidatedMsg);
    //}
}
void CIni::createLogFile()
{
    //Create Log folder
    std::string tempMkdir = "mkdir " + m_sLogFilePath;
    const char* m_sMkdirPath = tempMkdir.c_str();

    outFile.open(m_sLogFileFullPath, std::ios_base::app | std::ios_base::out);

    //Create the file if doesn't exist
    if (!outFile)
    {
        outFile.open(m_sLogFileFullPath, std::ios_base::app | std::ios_base::out);
        outFile << "\n";
        outFile.close();
    }

    outFile.close();
}

void CIni::writeLogFile(std::string msg)
{
    //Get the current time and date
    auto currentTime = []()
    {
        time_t now = time(0);

        // convert to string 
        char* dt = ctime(&now);
        return dt;
    };
    //Open Log File
    outFile.open(m_sLogFileFullPath, std::ios_base::app | std::ios_base::out);
    //Write  message
    outFile << std::setw(10) << std::left << msg << std::setw(60) << std::right << currentTime();
  
    outFile.close();
}
void CIni::readFile() 
{
    int headersIndex = { 0 };
    //count header vector size
    inFile.open(m_sIniFileName);
    while (getline(inFile, m_sNextLine))
    {
        if (m_sNextLine.find(']') != std::string::npos) {
            headerSize++;
        }
    }
    inFile.close();

    m_vHeaders.resize(headerSize);
    inFile.open(m_sIniFileName);
    if (!inFile) {
        std::cerr << "error: not open" << std::endl;
    }
    //read file//

    while (getline(inFile, m_sNextLine))
    {
        if (m_sNextLine.find('[') != std::string::npos) 
        {
            m_vHeaders[headersIndex] = (m_sNextLine);
            headersIndex++;
        }
    }
    inFile.close();
    writeLogFile(m_sReadMsg);
}

void CIni::writeFile(std::string* userInput, std::vector<std::string> m_vInputs)
{
    //Indices for vectors
    int j{ 0 };
    int i{ 0 };

    //Find index of section name lambda
    auto findSectionIndex = [](std::vector<std::string>& v, std::string& s)
    {
    int tempIndex{};
    auto it = find(v.begin(), v.end(), s);
    if (it != v.end())
    {
        tempIndex = it - v.begin();
    }
    return tempIndex;
    };
    auto writeSectionHeader = [](std::vector<std::string>& v, std::ostream& os, int sectionIndex, int& currentIndex)
    {
        //Check the previous header than section header
        if (currentIndex == sectionIndex - 1)
        {
            //Write -EOS-
            os << "-EOS-\n" << "ddd\n";
            //Increase index +1 to get the section header and write it
            currentIndex++;
            os << v[currentIndex] << std::endl;
        }
    };

    //Find the index of start of each section
    indexSectionA = findSectionIndex(m_vHeaders, m_vSectionNames[0]);
    indexSectionB = findSectionIndex(m_vHeaders, m_vSectionNames[1]);




    //Open file
    outFile.open(m_sIniFileName, std::ofstream::trunc);
    //Iterate through whole file
    while (outFile && i < m_vHeaders.size()) 
    {
        //Program's input
        if (userInput == NULL) {
            outFile << m_vHeaders[i] << "\n" << m_vInputs[j] << std::endl;
            //Write -EOS- before new section
            writeSectionHeader(m_vHeaders, outFile, indexSectionA, i);
            writeSectionHeader(m_vHeaders, outFile, indexSectionB, i);
            i++; j++;
        }
        //User's input
        else {
            i = (i == indexSectionA || i == indexSectionB) ? i += 1: i;
            std::cout << m_vHeaders[i] << std::endl;
            std::cin >> *userInput;
            outFile << m_vHeaders[i] << "\n" << *userInput << std::endl;
            i++;
        }
    }

    outFile << "-EOS-" << "\n" << "-EOF-" << std::endl;
    //Close file
    outFile.close();
    writeLogFile(m_sWriteMsg);

}

bool CIni::validateFile()
{
    //Validate
    //String to Int lambda
    auto toInt = [](std::string s)
    {
        int temp = stoi(s);
        return temp;
    };
    //String to uppercase lambda

    auto toUpper = [](std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    };

    //Indices for vectors and counter for -EOS-
    int i{ 0 };
    int j{ 0 };
    int m_IEosCounter{ 0 };

    //Delete section headers and resize vector
    m_vHeaders.erase(m_vHeaders.begin() + indexSectionB);
    m_vHeaders.erase(m_vHeaders.begin() + indexSectionA);
    m_vValData.resize(m_vHeaders.size());

    inFile.open(m_sIniFileName);
    
    //Store the data
    while (getline(inFile, m_sNextLine)) 
    {
        //Counter for -EOS-
        m_IEosCounter = (m_sNextLine == "-EOS-") ? m_IEosCounter += 1 : m_IEosCounter;
        if (m_IEosCounter >= 3) 
        {
            break;
        }
        //Change line when the header has the same value and store the data
        if (m_sNextLine == m_vHeaders[j])
        {
            getline(inFile, m_sNextLine);
            m_vValData[i] = m_sNextLine;
            i++;
            j++;
        }
    }

    //Data 
    
    unsigned m_vMaxThreads{ 10 };
    std::vector<int> m_vCodeLimits{ -5, 5 };
    std::vector<unsigned> m_vResolutionLimits{ 0 , 150 };
    std::vector<std::string> m_vRetrieveType{ "DISTANCE", "DEPTH" };
    std::vector<std::string> m_vCoverMapAlgorithm{ "BASIC", "CUSTOM" };
    std::vector<std::string> m_vIgnoreMissignLoses{ "YES", "NO" };
    std::vector<std::string> m_vMaxRadiusUnits{ "KM", "M" };

    std::vector<bool> m_vValidationCheck( m_vValData.size()-1);
    //[VERSION]
    m_vValidationCheck[0] = (m_vValData[0] == "2.7.8") ? 1 : 0;
    //[LOG FOLDER]
    m_vValidationCheck[1] = (m_vValData[1] == m_sLogFilePath) ? 1 : 0;
    //[ LOG FILE ]
    m_vValidationCheck[2] = (m_vValData[2] == m_sLogFileFullPath) ? 1 : 0;
    //[ MAX THREADS ]
    m_vValidationCheck[3] = (toInt(m_vValData[3]) >= 0 && toInt(m_vValData[3]) <= m_vMaxThreads) ? 1 : 0;
    //[ MIN CODE ]
    m_vValidationCheck[4] = (toInt(m_vValData[4]) >= m_vCodeLimits[0] && (toInt(m_vValData[4]) <= m_vCodeLimits[1])) ? 1 : 0;
    //[ MAX CODE ]
    m_vValidationCheck[5] = (toInt(m_vValData[5]) >= m_vCodeLimits[0] && toInt(m_vValData[5]) <= m_vCodeLimits[1]) ? 1 : 0;
    //[ RESOLUTION ]
    m_vValidationCheck[6] = (toInt(m_vValData[6]) >= (m_vResolutionLimits[0]) && toInt(m_vValData[6]) <= (m_vResolutionLimits[1])) ? 1 : 0;
    //[RETRIEVE TYPE]
    m_vValidationCheck[7] = (toUpper(m_vValData[7]) == (toUpper(m_vRetrieveType[0])) || toUpper(m_vValData[7]) == toUpper(m_vRetrieveType[1])) ? 1 : 0;
    //[COVERAGE MAP ALGORITHM]
    m_vValidationCheck[8] = (toUpper(m_vValData[8]) == toUpper(m_vCoverMapAlgorithm[0]) || toUpper(m_vValData[8]) == toUpper(m_vCoverMapAlgorithm[1])) ? 1 : 0;
    //[IGNORE TX WITH MISSING LOSSES]
    m_vValidationCheck[9] = (toUpper(m_vValData[9]) == toUpper(m_vIgnoreMissignLoses[0]) || toUpper(m_vValData[9]) == toUpper(m_vIgnoreMissignLoses[1])) ? 1 : 0;
    //[ MAX RADIUS UNIT ]
    m_vValidationCheck[10] = (toUpper(m_vValData[10]) == toUpper(m_vMaxRadiusUnits[0]) || toUpper(m_vValData[10]) == toUpper(m_vMaxRadiusUnits[1])) ? 1 : 0;

    bool m_bFlag = true;

    for (auto isTrue : m_vValidationCheck)
    {
        m_bFlag = (isTrue == false) ? false : m_bFlag;
        std::cout << isTrue << std::endl;

    }
    return m_bFlag;
    writeLogFile(m_sIniFileName);

}
