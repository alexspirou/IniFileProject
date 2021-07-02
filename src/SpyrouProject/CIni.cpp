#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <filesystem>
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
    const std::string m_sLogFileFullPath = m_sLogFilePath + "\\" + m_sLogFileName;

    //File in/out objects
    std::ifstream inFile{};
    std::ofstream outFile{};

    outFile.open(m_sLogFileFullPath, std::ios_base::app | std::ios_base::out);

    //Create the file if doesn't exist
    if (!outFile)
    {
        system(m_sMkdirPath);
        outFile.open(m_sLogFileFullPath, std::ios_base::app | std::ios_base::out);
        //outFile << "\n";
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
    //File in/out objects
    std::ifstream inFile{};
    std::ofstream outFile{};
    //Open Log File
    const std::string m_sLogFileFullPath = m_sLogFilePath + "\\" + m_sLogFileName;

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
        outFile <<  std::endl;
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

size_t CIni::readIniFile()
{
    writeLogFile("Read has started", 1, 0);
    size_t m_szHeaderSize{ 0 };
    int i = { 0 };
    //count header vector size
    //File in/out objects
    std::ifstream inFile{};
    std::ofstream outFile{};
    //Next string for iterate the file
    std::string m_sNextLine{};
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



    //Send message to logfile
    writeLogFile("Read completed", 1, 0);

    return m_szHeaderSize;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
///////////////////////////// PUBLIC ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Write to the .ini file the desirable data either from program or user
/////////////////////////////////////////////////////////////////////////////

void CIni::writeIniFile(bool userInput)
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

    //Write eos lambda
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

   //Check if 
    auto isDigit = [](char& c)
    {
        return std::isdigit(c);
    };
    //File in/out objects
    std::ifstream inFile{};
    std::ofstream outFile{};

    //Next string for iterate the file
    std::string m_sNextLine{};
    //Indices for vectors
    //Header
    int j{ 0 };
    //Values
    int i{ 0 };

    std::vector<std::string> m_vHeaders{};
    //Resize with the number of headers names
    size_t size = readIniFile();
    m_vHeaders.resize(size);

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
    writeLogFile("Write has started", 1, 0);



    //Section Names for special use
    std::string m_sFirstHeaderName = { "[ -GENERAL SECTION- ]" };
    std::vector<std::string> m_vSectionNames{ "[ -A SECTION- ]" , "[ -B SECTION- ]" };
    std::vector<std::string> m_vInputIntOnly{ "[ MAX THREADS ]","[ MIN CODE ]","[ MAX CODE ]","[ RESOLUTION ]" };
    std::string m_sVersionHeader = { "[ VERSION ]" };

    //Find the index that starts each section
    int m_iIndexSectionA = findHeaderIndex(m_vHeaders, m_vSectionNames[0]);
    int m_iIndexSectionB = findHeaderIndex(m_vHeaders, m_vSectionNames[1]);
    //Find version index
    const int m_iIndexVersion = findHeaderIndex(m_vHeaders, m_sVersionHeader);
    //find headers that take integer input
    const int m_iIndexMaxThreads = findHeaderIndex(m_vHeaders, m_vInputIntOnly[0]);
    const int m_iIndexMinCode = findHeaderIndex(m_vHeaders, m_vInputIntOnly[1]);
    const int m_iIndexMaxCode = findHeaderIndex(m_vHeaders, m_vInputIntOnly[2]);
    const int m_iIndexResolution = findHeaderIndex(m_vHeaders, m_vInputIntOnly[3]);

    //Vector for  values
    std::vector<std::string> m_vHeaderValues(m_vHeaders.size() - 2);

     //Values for each section
    m_vHeaderValues[0] = "2.7.8";                                                       //[ VERSION ]
    m_vHeaderValues[1] = m_sLogFilePath;                                                //[ LOG FOLDER ]
    m_vHeaderValues[2] = m_sLogFilePath + "\\" + m_sLogFileName;                        //[ LOG FILE ]
    m_vHeaderValues[3] = "40";                                                           //[ MAX THREADS ]
    m_vHeaderValues[4] = "1";                                                           //[ MIN CODE ]
    m_vHeaderValues[5] = "2";                                                           //[ MAX CODE ]
    m_vHeaderValues[6] = "150";                                                         //[ RESOLUTION ]
    m_vHeaderValues[7] = "DISTANCE";                                                    //[ RETRIEVE TYPE ]
    m_vHeaderValues[8] = "BASIC";                                                       //[ COVERAGE MAP ALGORITHM ]
    m_vHeaderValues[9] = "NO";                                                          //[ IGNORE TX WITH MISSING LOSSES ]
    m_vHeaderValues[10] = "km";                                                         //[ MAX RADIUS UNIT ]
    m_vHeaderValues[11] = "-106.0,100,150.0,10000.0,1,9.0,1,-98.0,2.0,-98.0,3.0,1,0,0"; //[ BASIC PARAMETERS ]
    //Open file in replace mode
    outFile.open(m_sIniFileName, std::ofstream::trunc);
    i = 0;
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
        if (userInput == false)
        {
            //Write to the .ini file the headers names and the values
            outFile << m_vHeaders[i] << "\n" << m_vHeaderValues[j] << std::endl;
            j++;
        }
        //User's input
        else
        {
            //Check if the header is version to write header and the program's value
            if (i == m_iIndexVersion)
            {
                outFile << m_vHeaders[i] << "\n" << m_vHeaderValues[0] << std::endl;
            }
            //Check if index = section names or version header to increase by 1 to skip them
            i = (i == m_iIndexSectionA || i == m_iIndexSectionB || i == m_iIndexVersion) ? i += 1 : i;

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
                //If the input have not to be an integer
                std::getline(std::cin, m_sUserInput);

            }
            //Write the header and values
            outFile << m_vHeaders[i] << "\n" << m_sUserInput << std::endl;

        }
        //Write the section names
        

        writeSectionHeader(m_vHeaders, outFile, m_iIndexSectionA, i);
        writeSectionHeader(m_vHeaders, outFile, m_iIndexSectionB, i);
        i++;
    }
    outFile << "-EOS-" << "\n" << "-EOF-" << std::endl;
    //Close file
    outFile.close();
    writeLogFile("Write completed", 1, 0);
}