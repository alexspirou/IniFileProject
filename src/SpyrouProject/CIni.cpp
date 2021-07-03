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
    std::vector<std::string> m_defaultHeaders(15);
    m_defaultHeaders[0]  =  "[ -GENERAL SECTION- ]";
    m_defaultHeaders[1]  =  "[ VERSION ]";
    m_defaultHeaders[2]  =  "[ LOG FOLDER ]";
    m_defaultHeaders[3]  =  "[ LOG FILE ]";
    m_defaultHeaders[4]  =  "[ MAX THREADS ]";
    m_defaultHeaders[5]  =  "[ -A SECTION- ]";
    m_defaultHeaders[6]  =  "[ MIN CODE ]";
    m_defaultHeaders[7]  =  "[ MAX CODE ]";
    m_defaultHeaders[8]  =  "[ RESOLUTION ]";
    m_defaultHeaders[9]  =  "[ RETRIEVE TYPE ]";
    m_defaultHeaders[10] =  "[ COVERAGE MAP ALGORITHM ]";
    m_defaultHeaders[11] =  "[ IGNORE TX WITH MISSING LOSSES ]";
    m_defaultHeaders[12] =  "[ -B SECTION- ]";
    m_defaultHeaders[13] =  "[ MAX RADIUS UNIT ]";
    m_defaultHeaders[14] =  "[ BASIC PARAMETERS ]";
    //Check if a header is missing
    if (readIniFile() < 15)
    {
        std::ofstream outFile{};
        unsigned indexDefaultHeaders{ 0 };

        outFile.open(m_sIniFileName);

        while (indexDefaultHeaders < 15) 
        {
            outFile << m_defaultHeaders[indexDefaultHeaders] << std::endl;
            indexDefaultHeaders++;
        }

        outFile.close();
    }
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
    int indexHeader{ 0 };

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
            m_vHeaders[indexHeader] = (m_sNextLine);
            indexHeader++;
        }
    }
    inFile.close();
    writeLogFile("Write has started", 1, 0);

    //Vector for  values
    std::vector<std::string> m_vOptions(m_vHeaders.size() - 2);

     //Values for each section
    m_vOptions[0] = "2.7.8";                                                       //[ VERSION ]
    m_vOptions[1] = m_sLogFilePath;                                                //[ LOG FOLDER ]
    m_vOptions[2] = m_sLogFilePath + "\\" + m_sLogFileName;                        //[ LOG FILE ]
    m_vOptions[3] = "41";                                                           //[ MAX THREADS ]
    m_vOptions[4] = "1";                                                           //[ MIN CODE ]
    m_vOptions[5] = "2";                                                           //[ MAX CODE ]
    m_vOptions[6] = "150";                                                         //[ RESOLUTION ]
    m_vOptions[7] = "DISTANCE";                                                    //[ RETRIEVE TYPE ]
    m_vOptions[8] = "BASIC";                                                       //[ COVERAGE MAP ALGORITHM ]
    m_vOptions[9] = "NO";                                                          //[ IGNORE TX WITH MISSING LOSSES ]
    m_vOptions[10] = "km";                                                         //[ MAX RADIUS UNIT ]
    m_vOptions[11] = "-106.0,100,150.0,10000.0,1,9.0,1,-98.0,2.0,-98.0,3.0,1,0,0"; //[ BASIC PARAMETERS ]

    //Open file in replace mode
    outFile.open(m_sIniFileName, std::ofstream::trunc);

    int indexOptions{ 0 };
    indexHeader = 0;
  
    //Iterate through whole file
    while (outFile && indexHeader < m_vHeaders.size())
    {
        //Program's input
        if (userInput == 0)
        {
            //If the header doesn't need an option.
            if (m_vHeaders[indexHeader] == "[ -GENERAL SECTION- ]" || m_vHeaders[indexHeader] == "[ -A SECTION- ]" || m_vHeaders[indexHeader] == "[ -B SECTION- ]")
            {
                outFile << m_vHeaders[indexHeader] << std::endl;
                indexHeader++;
            }
            //If -EOS- needs to be written
            else if (m_vHeaders[indexHeader] == "[ MAX THREADS ]" || m_vHeaders[indexHeader] == "[ IGNORE TX WITH MISSING LOSSES ]")
            {
                outFile << m_vHeaders[indexHeader] << "\n" << m_vOptions[indexOptions] << "\n" << "-EOS-" << "\n" << std::endl;
                indexHeader++; indexOptions++;
            }
            //Header and option
            else
            {
                outFile << m_vHeaders[indexHeader] << "\n" << m_vOptions[indexOptions] << std::endl;
                indexHeader++; indexOptions++;
            }
        }
        //User's input
        if(userInput == 1)
        {
            std::string m_sUserInput{};
            if (m_vHeaders[indexHeader] == "[ -GENERAL SECTION- ]" || m_vHeaders[indexHeader] == "[ -A SECTION- ]" || m_vHeaders[indexHeader] == "[ -B SECTION- ]")
            {
                outFile << m_vHeaders[indexHeader] << std::endl;
                indexHeader++;
            }
            //Check if the header is version to write header and the program's value
            if (m_vHeaders[indexHeader] == "[ VERSION ]")
            {
                outFile << m_vHeaders[indexHeader] << "\n" << m_vOptions[0] << std::endl;
                indexHeader++;
            }
            //Print in console the header names
            std::cout << m_vHeaders[indexHeader] << std::endl;

            //Check if the input must be an integer
            if (m_vHeaders[indexHeader] == "[ MIN CODE ]" || m_vHeaders[indexHeader] == "[ MAX CODE ]" || m_vHeaders[indexHeader] == "[ RESOLUTION ]")
            {
                //Get user's input
                std::getline(std::cin, m_sUserInput);
                while (!std::all_of(m_sUserInput.begin(), m_sUserInput.end(), isDigit))
                {
                    //Loop untill the user enter an integer
                    std::cout << "Input must be an integer. " << std::endl;
                    std::cout << m_vHeaders[indexHeader] << std::endl;
                    std::getline(std::cin, m_sUserInput);
                }
            }
            //If the input doesn't need to be an integer
            else
            {
                std::getline(std::cin, m_sUserInput);

            }
            //Write the header and the option with -EOS-
            if (m_vHeaders[indexHeader] == "[ MAX THREADS ]" || m_vHeaders[indexHeader] == "[ IGNORE TX WITH MISSING LOSSES ]")
            {
                outFile << m_vHeaders[indexHeader] << "\n" << m_sUserInput << "\n" << "-EOS-" << "\n" << std::endl;
            }
            //Write header and option
            else
            {
                outFile << m_vHeaders[indexHeader] << "\n" << m_sUserInput << std::endl;

            }
            indexHeader++;

        }
                
    }
    //End of file write -EOS and -EOF-
    outFile << "-EOS-" << "\n" << "-EOF-" << std::endl;
    //Close file
    outFile.close();
    writeLogFile("Write completed", 1, 0);
}