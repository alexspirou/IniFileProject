#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <ctime>
#include <iomanip>
#include "CIni.h"

////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////// 
//--------------------------------------------------------------------------------//
// CIni
//--------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
///////////////////////////// PUBLIC ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

CIni::CIni()
{
    auto removeBrackets = [](std::vector<std::string>& v)
    {
        int i = 0;
        for (auto s : v)
        {
            //Erase []
            s.erase(std::remove(s.begin(), s.end(), '['), s.end());
            s.erase(std::remove(s.begin(), s.end(), ']'), s.end());
            //Assign to the vector
            v[i] = s;
            i++;
        }
        return v;
    };
    //Create log file
    createLogFile();
    //Enter \n for change the line from the last run
    writeLogFile("\n", 0, 0);
    outFile << "\n" << std::setfill('-') << std::setw(100) << std::endl;
    writeLogFile("Program has started", 1, 1);

    //createLogFile();
    readFile();
    //Resize vector with header's vector size - section names(2)
    m_vHeaderValues.resize(m_vHeaders.size() - m_vSectionNames.size());

    //Values for each section
    m_vHeaderValues[0] = "2.7.8";                                                       //[ VERSION ]
    m_vHeaderValues[1] = m_sLogFilePath;                                                //[ LOG FOLDER ]
    m_vHeaderValues[2] = m_sLogFilePath + "\\" + m_sLogFileName;                        //[ LOG FILE ]
    m_vHeaderValues[3] = "2";                                                           //[ MAX THREADS ]
    m_vHeaderValues[4] = "1";                                                           //[ MIN CODE ]
    m_vHeaderValues[5] = "2";                                                           //[ MAX CODE ]
    m_vHeaderValues[6] = "150";                                                         //[ RESOLUTION ]
    m_vHeaderValues[7] = "DISTANCE";                                                    //[ RETRIEVE TYPE ]
    m_vHeaderValues[8] = "BASIC";                                                       //[ COVERAGE MAP ALGORITHM ]
    m_vHeaderValues[9] = "NO";                                                          //[ IGNORE TX WITH MISSING LOSSES ]
    m_vHeaderValues[10] = "km";                                                         //[ MAX RADIUS UNIT ]
    m_vHeaderValues[11] = "-106.0,100,150.0,10000.0";                                   //[ BASIC PARAMETERS ]

    //Remove brackets if exist
    m_vHeaderValues = removeBrackets(m_vHeaderValues);

    //Resize vector with header's vector size - section names size (2)
    m_vDefaultValues.resize(m_vHeaders.size() - m_vSectionNames.size());
    //Default values if validation failed
    m_vDefaultValues[0] = "2.7.8";                                                       //[ VERSION ]
    m_vDefaultValues[1] = "--";                                                          //[ LOG FOLDER ]
    m_vDefaultValues[2] = "C:\\Log\LogFile.dbg";                                         //[ LOG FILE ]
    m_vDefaultValues[3] = "1";                                                           //[ MAX THREADS ]
    m_vDefaultValues[4] = "-1";                                                          //[ MIN CODE ]
    m_vDefaultValues[5] = "-1";                                                          //[ MAX CODE ]
    m_vDefaultValues[6] = "100";                                                         //[ RESOLUTION ]
    m_vDefaultValues[7] = "DISTANCE";                                                    //[ RETRIEVE TYPE ]
    m_vDefaultValues[8] = "BASIC";                                                       //[ COVERAGE MAP ALGORITHM ]
    m_vDefaultValues[9] = "YES";                                                         //[ IGNORE TX WITH MISSING LOSSES ]
    m_vDefaultValues[10] = "km";                                                         //[ MAX RADIUS UNIT ]
    m_vDefaultValues[11] = "-106.0,100,150.0,10000.0,1,9.0,1,-98.0,2.0,-98.0,3.0,1,0,0"; //[ BASIC PARAMETERS ]

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
///////////////////////////// PRIVATE ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Create the log folder and the log file if it doesn't exist
/////////////////////////////////////////////////////////////////////////////

void CIni::createLogFile()
{

    //Create Log folder
    std::string tempMkdir = "mkdir " + m_sLogFilePath;
    const char* m_sMkdirPath = tempMkdir.c_str();

    system(m_sMkdirPath);
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
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
///////////////////////////// PUBLIC ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Write events and the current date/time  in the log file 
/////////////////////////////////////////////////////////////////////////////

void CIni::writeLogFile(std::string msg, bool bTime, bool bStartsEnd)
{

    //Get the current time and date
    auto currentTime = []()
    {
        //Take the time at this moment
        time_t now = time(0);
        // convert to string 
        char* dt = ctime(&now);
        return dt;
    };
    
    //Open Log File
    outFile.open(m_sLogFileFullPath, std::ios_base::app | std::ios_base::out);
    
    //Write  message
    if (bTime && !bStartsEnd)
    {
        //Write the message to logfile
        outFile << std::setw(10) << std::left << m_sIniFileName << " " << msg << std::setw(30 - msg.length() + 65) << std::right << currentTime();
    }
    else if (!bTime && !bStartsEnd)
    {
        //Write a message withou time
        outFile << std::endl;
    }
    else
    {
        //Write a message  when the app starts-stop
        outFile << std::setw(10) << std::left << msg << " " << std::setw(30 - msg.length() + m_sIniFileName.length() + 65) << std::right << currentTime();
    
    }
    outFile.close();
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
///////////////////////////// PUBLIC ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Read the .ini file and store the headers in a vector
/////////////////////////////////////////////////////////////////////////////

void CIni::readFile()
{
    writeLogFile("Read has started", 1, 0);

    int i = { 0 };
    //count header vector size
    inFile.open(m_sIniFileName);
    while (getline(inFile, m_sNextLine))
    {
        //Increased only if is inside in []
        if (m_sNextLine.find('[') != std::string::npos && m_sNextLine.find(']') != std::string::npos)
        {
            m_szHeaderSize++;
        }
    }
    inFile.close();

    //Resize with the number of headers names
    m_vHeaders.resize(m_szHeaderSize);

    inFile.open(m_sIniFileName);
    if (!inFile) 
    {
        std::cerr << "error: not open" << std::endl;
    }
    //read file per line

    while (getline(inFile, m_sNextLine))
    {
        if (m_sNextLine.find('[') != std::string::npos && m_sNextLine.find(']') != std::string::npos)
        {
            //Stores only if is inside in []
            m_vHeaders[i] = (m_sNextLine);
            i++;
        }
    }
    inFile.close();

    //Send message to logfile
    writeLogFile("Read completed", 1, 0);

}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
///////////////////////////// PUBLIC ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Write to the .ini file the desirable data either from program or user
/////////////////////////////////////////////////////////////////////////////

void CIni::writeFile(bool userInput, std::vector<std::string> m_vInputs)
{

    //Find index of section name lambda
    auto findHeaderIndex = [](std::vector<std::string>& v, std::string& s)
    {
        int tempIndex{};
        //Search vector for section name
        auto it = find(v.begin(), v.end(), s);

        if (it != v.end())
        {
            //Finds the index
            tempIndex = it - v.begin();
        }
        return tempIndex;
    };
    auto writeSectionHeader = [](std::vector<std::string>& vHeaders, std::ostream& os, int sectionIndex, int& currentIndex)
    {
        //Check the previous header than section header
        if (currentIndex == sectionIndex - 1)
        {
            //Write -EOS-
            os << "-EOS-\n" << "\n";
            //Increase index +1 to get the section header and write it
            currentIndex++;
            os << vHeaders[currentIndex] << std::endl;
        }
    };

    auto isDigit = [](char& c)
    {
        return std::isdigit(c);
    };

    writeLogFile("Write has started", 1, 0);

    //Indices for vectors
    int j{ 0 };
    int i{ 0 };

    //Find the index that starts each section
    m_iIndexSectionA = findHeaderIndex(m_vHeaders, m_vSectionNames[0]);
    m_iIndexSectionB = findHeaderIndex(m_vHeaders, m_vSectionNames[1]);
    
    //find headers that take integer input
    int m_iIndexMaxThreads = findHeaderIndex(m_vHeaders, m_vInputIntOnly[0]);
    int m_iIndexMinCode = findHeaderIndex(m_vHeaders, m_vInputIntOnly[1]);
    int m_iIndexMaxCode = findHeaderIndex(m_vHeaders, m_vInputIntOnly[2]);
    int m_iIndexResolution = findHeaderIndex(m_vHeaders, m_vInputIntOnly[3]);

    //int m_iIndexVersion = findHeaderIndex(m_vHeaders, m_sVersionHeader);

    //Open file in replace mode
    outFile.open(m_sIniFileName, std::ofstream::trunc);
    //Iterate through whole file
    while (outFile && i < m_vHeaders.size())
    {
        std::string m_sUserInput;
        if (m_vHeaders[i] == m_sFirstHeaderName)
        {
            //Write the .ini file and icrease index by one to reach next header name
            outFile << m_vHeaders[i] << std::endl;
            i++;

        }
        //Program's input
        if (userInput == NULL)
        {
            //Write to the .ini file the headers names and the values
            outFile << m_vHeaders[i] << "\n" << m_vInputs[j] << std::endl;
            j++;
        }
        //User's input
        else
        {
            //Check if index = section names or version header to increase by 1 to skip 
            i = (i == m_iIndexSectionA || i == m_iIndexSectionB) ? i += 1 : i;
            //Print in console the header names
            std::cout << m_vHeaders[i] << std::endl;

            //Check if the input must be an integer
            if (i == m_iIndexMaxThreads || i == m_iIndexMinCode || i == m_iIndexMaxCode || i == m_iIndexResolution)
            {
                //Get user's input
                std::getline(std::cin, m_sUserInput);
                while (!std::all_of(m_sUserInput.begin(), m_sUserInput.end(), isDigit))
                {
                    //Loop untill the user enter an integer
                    std::cout << "Input must be an integer. " << std::endl;
                    std::cout << m_vHeaders[i] << std::endl;
                    std::getline(std::cin, m_sUserInput);
                }
            }
            else
            {
                std::getline(std::cin, m_sUserInput);

            }
            outFile << m_vHeaders[i] << "\n" << m_sUserInput << std::endl;

        }
        writeSectionHeader(m_vHeaders, outFile, m_iIndexSectionA, i);
        writeSectionHeader(m_vHeaders, outFile, m_iIndexSectionB, i);
        i++;
    }
    outFile << "-EOS-" << "\n" << "-EOF-" << std::endl;
    //Close file
    outFile.close();
    writeLogFile("Write completed", 1, 0);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
///////////////////////////// PUBLIC ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Read the .ini file, store the values and validate them, retunrs true if the
// validation is succesfull
/////////////////////////////////////////////////////////////////////////////
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

    //Trasnform version to int
    auto toIntDot = [](std::string s)
    {
        s.erase(std::remove(s.begin(), s.end(), '.'), s.end());
        int temp = stoi(s);
        return temp;
    };

    //Indices for vectors and counter for -EOS-
    int i{ 0 };
    int j{ 0 };
    int m_IEosCounter{ 0 };

    //Delete section headers and resize vector
    m_vHeaders.erase(m_vHeaders.begin() + m_iIndexSectionB);
    m_vHeaders.erase(m_vHeaders.begin() + m_iIndexSectionA);
    m_vHeaders.erase(m_vHeaders.begin() + 0);

    //Resize with new size of m_vHeaders vector
    m_vValData.resize(m_vHeaders.size() + 1);

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
            //Change line to access the values instead of headers
            getline(inFile, m_sNextLine);
            //Store the data in the vector
            m_vValData[i] = m_sNextLine;
            i++; j++;
        }
    }

    //Validated Restrictions 

    std::vector<unsigned> m_vVersionLimits{ 270, 280 };                     //[ VERSION ]
    unsigned m_vMaxThreads{ 10 };                                           //[ MAX THREADS ]
    std::vector<int> m_vCodeLimits{ -5, 5 };                                //[ MIN CODE ] [ MAX CODE ]
    std::vector<unsigned> m_vResolutionLimits{ 0 , 150 };                   //[ RESOLUTION ]
    std::vector<std::string> m_vRetrieveType{ "DISTANCE", "DEPTH" };        //[ RETRIEVE TYPE ]
    std::vector<std::string> m_vCoverMapAlgorithm{ "BASIC", "CUSTOM" };     //[ COVERAGE MAP ALGORITHM ]
    std::vector<std::string> m_vIgnoreMissignLoses{ "YES", "NO" };          //[ IGNORE TX WITH MISSING LOSSES ]
    std::vector<std::string> m_vMaxRadiusUnits{ "km", "m" };                //[ MAX RADIUS UNIT ]

    //Write in the log file
    writeLogFile("Validation", 1, 0);

    std::vector<bool> m_vValidationCheck(m_vValData.size() - 1);
    //[VERSION]                         
    m_vValidationCheck[0] = (toIntDot(m_vValData[0]) >= m_vVersionLimits[0] && toIntDot(m_vValData[0]) <= m_vVersionLimits[1]) ? 1 : 0;
    //[LOG FOLDER]
    m_vValidationCheck[1] = (m_vValData[1] == m_sLogFilePath) ? 1 : 0;
    //[ LOG FILE ]
    m_vValidationCheck[2] = (m_vValData[2] == m_sLogFileFullPath) ? 1 : 0;
    //[ MAX THREADS ]
    m_vValidationCheck[3] = (toInt(m_vValData[3]) >= 0 && toInt(m_vValData[3]) <= m_vMaxThreads) ? 1 : 0;
    //[ MIN CODE ]
    m_vValidationCheck[4] = (toInt(m_vValData[4]) >= m_vCodeLimits[0] && (toInt(m_vValData[4]) <= m_vCodeLimits[1]) && toInt(m_vValData[4]) <= toInt(m_vValData[5])) ? 1 : 0;
    //[ MAX CODE ]
    m_vValidationCheck[5] = (toInt(m_vValData[5]) >= m_vCodeLimits[0] && toInt(m_vValData[5]) <= m_vCodeLimits[1] && toInt(m_vValData[5]) >= toInt(m_vValData[4])) ? 1 : 0;
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
    //[ BASIC PARAMETERS ]
    m_vValidationCheck[11] = ((m_vValData[11]) == m_vHeaderValues[11]) ? 1 : 0;


    bool m_bFlag = true;
    i = 0;
    for (auto isTrue : m_vValidationCheck)
    {

        //Check if at least one of elements is false and set it to a bool flag
        m_bFlag = (isTrue != true) ? false : m_bFlag;
        //Write the section name that is not valid
        if (!isTrue) { writeLogFile(m_vHeaders[i] + " header not valid", 1, 0); }
        i++;
    }

    //Increase error handler in case that function complete
    m_iErrorHandler++;

    //return the flag
    return m_bFlag;
    writeLogFile("Validation completed", 1, 0);

}
