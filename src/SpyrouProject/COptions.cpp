#include "COptions.h"
#include "CIni.h"
#include <algorithm>
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////// 
//--------------------------------------------------------------------------------//
// COptions
//--------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
///////////////////////////// PUBLIC ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
COptions::COptions()
{

}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
///////////////////////////// PUBLIC ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Read the .ini file, store the values and validate them. If the inputs are 
// not valid, default values are written in .ini file. Return true if the in
// -puts are valid.
/////////////////////////////////////////////////////////////////////////////
bool COptions::isValid()
{
    //File stream objects
    std::ifstream m_inFile{};
    std::ofstream m_outFile{};
    ////Validate

    //Array of string to double per  comma
    auto toDouble = [](std::string stringToDouble)
    {
        //Try for basic parameters string to double per comma
        std::vector <int> vPreviousComma;
        //indices 
        int i = 0;
        //Previous comma
        int prevIndexComma = 0;
        //Next comma
        int indexComma = 0;

        while (i < stringToDouble.length())
        {
            //find the comma
            auto it = std::find(stringToDouble.begin(), stringToDouble.end(), ',');

            //store the previous state
            prevIndexComma = indexComma;

            //Count the difference
            indexComma = it - stringToDouble.begin();

            //Erase comma to find the next
            if (indexComma <= stringToDouble.size())
            {
                stringToDouble.erase(stringToDouble.begin() + indexComma);
            }
            //Store the previous state
            vPreviousComma.push_back(prevIndexComma);
            //Set count as the previous state to exit the loop
            i = prevIndexComma;
        }
        //Vector store the values without comma
        std::vector<std::string> vStringDoubleValues(vPreviousComma.size());

        //Index for vPreviousComma vector to start for the second element
        i = 1;
        int j = 0;
        int prevState = 0;

        //Outter loop equal the comma size
        for (int x = 0; x < vPreviousComma.size() - 1; x++)
        {
            for (int in = prevState; in < vPreviousComma[i]; in++)
            {
                //Inner loop that store the value betwwen previous and next comma
                vStringDoubleValues[j].push_back(stringToDouble[in]);
                //Set current comma to previous comma
                prevState = vPreviousComma[i];
            }
            //Increase index for previous comma vector and string vStringDoubleValues vector
            i++; j++;
        }

        std::vector<double> vDoubleValues(vStringDoubleValues.size() - 1);
        i = 0;

        while (i < vDoubleValues.size())
        {
            //Conver to double and store to double vector
            vDoubleValues[i] = stod(vStringDoubleValues[i]);
            i++;
        }

        return vDoubleValues;
    };

    //Find max lambda
    auto findMax = [](std::vector<double> doubleVec)
    {
        double max = doubleVec[0];
        for (auto element : doubleVec)
        {
            max = (max <= element) ? max = element : max;
        }
        return max;
    };

    //Find min lambda
    auto findMin = [](std::vector<double> doubleVec)
    {
        double min = doubleVec[0];
        for (auto element : doubleVec)
        {
            min = (min >= element) ? min = element : min;
        }
        return min;
    };

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
    
    CIni m_IniObj;

    //Index for vector
    unsigned indexHeader{ 0 };
    //Next string for iterate the file
    std::string m_sNextLine{};
    //Vector for headers
    size_t m_iHeadersSize = m_IniObj.readIniFile();
    std::vector<std::string> m_vHeaders(m_iHeadersSize);

    //READ FILE
    m_inFile.open(m_sIniFileName);
    //
    while (getline(m_inFile, m_sNextLine))
    {
        if (m_sNextLine.find('[') != std::string::npos && m_sNextLine.find(']') != std::string::npos)
        {
            //Stores only if is inside in []
            m_vHeaders[indexHeader] = (m_sNextLine);
            indexHeader++;
        }
    }
    m_inFile.close();

    //CHECK FOR VALIDATION
    
    //Vector for validation data
    std::vector<std::string> m_vValData(m_iHeadersSize - 3);
    //Idex and counter for -EOS-
    unsigned indexValData{ 0 };
    unsigned m_IEosCounter{ 0 };
    indexHeader = 0;

    m_inFile.open(m_sIniFileName);

    //Store the data
    while (getline(m_inFile, m_sNextLine))
    {
        //Counter for -EOS-
        m_IEosCounter = (m_sNextLine == "-EOS-") ? m_IEosCounter += 1 : m_IEosCounter;
        if (m_IEosCounter >= 3)
        {

            break;
        }
        //Skip when a header doesn't need an option
        if (m_sNextLine == "[ -GENERAL SECTION- ]" || m_sNextLine == "[ -A SECTION- ]" || m_sNextLine == "[ -B SECTION- ]")
        {
            getline(m_inFile, m_sNextLine);
            indexHeader++;
        }
        //Change line when the header has the same value and store the data
        if (m_sNextLine == m_vHeaders[indexHeader] )
        {
          
            //Change line to access the values instead of headers
            getline(m_inFile, m_sNextLine);
            //Store the data in the vector
            m_vValData[indexValData] = m_sNextLine;
            indexValData++; indexHeader++;
        }
    }

    m_inFile.close();

    //Validated Restrictions 

    const std::vector<unsigned> m_vVersionLimits{ 270, 280 };                     //[ VERSION ]
    const std::string m_sLogFolderPath{"C:\\Application_Log"};                    //[ LOG FOLDER ]
    const std::string m_sLogFilePath{ "C:\\Application_Log\\Logfile.dbg"};        //[ LOG FILE ]                                                                                 
    const unsigned m_vMaxThreads{ 10 };                                           //[ MAX THREADS ]
    const std::vector<int> m_vCodeLimits{ -5, 5 };                                //[ MIN CODE ] [ MAX CODE ]
    const std::vector<unsigned> m_vResolutionLimits{ 0 , 150 };                   //[ RESOLUTION ]
    const std::vector<std::string> m_vRetrieveType{ "DISTANCE", "DEPTH" };        //[ RETRIEVE TYPE ]
    const std::vector<std::string> m_vCoverMapAlgorithm{ "BASIC", "CUSTOM" };     //[ COVERAGE MAP ALGORITHM ]
    const std::vector<std::string> m_vIgnoreMissignLoses{ "YES", "NO" };          //[ IGNORE TX WITH MISSING LOSSES ]
    const std::vector<std::string> m_vMaxRadiusUnits{ "km", "m" };                //[ MAX RADIUS UNIT ]
    const std::vector<double> m_vBasicParametersLimits{ 10000, -500, 14 };        //[ BASIC PARAMETERS ]

    //Find min max for check the basicparameters limits
    const double valueMax = findMax(toDouble(m_vValData[11]));
    const double valueMin = findMin(toDouble(m_vValData[11]));

    //Write in the log file
    m_IniObj.writeLogFile("Validation", 1, 0);
    std::vector<bool> m_vValidationCheck(m_vValData.size());

    //[VERSION]                         
    m_vValidationCheck[0] = (toIntDot(m_vValData[0]) >= m_vVersionLimits[0] && toIntDot(m_vValData[0]) <= m_vVersionLimits[1]) ? 1 : 0;
    //[LOG FOLDER]
    m_vValidationCheck[1] = (m_vValData[1] == m_sLogFolderPath) ? 1 : 0;
    //[ LOG FILE ]
    m_vValidationCheck[2] = (m_vValData[2] == m_sLogFilePath) ? 1 : 0;
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
    m_vValidationCheck[11] = (valueMax <= m_vBasicParametersLimits[0] && valueMin >= m_vBasicParametersLimits[1] && toDouble(m_vValData[11]).size() == m_vBasicParametersLimits[2]) ? 1 : 0;
    
    bool m_bFlag = true;
    
    for (auto isTrue : m_vValidationCheck)
    {
        //Check if at least one of elements is false and set it to a bool flag
        m_bFlag = (isTrue == false) ? false : m_bFlag;
    }
    
    indexHeader = 0;
    //If not valid write the default values
    if (m_bFlag == false)
    {
        m_outFile.open(m_sIniFileName, std::ofstream::trunc);
        
        while (m_outFile && indexHeader < m_vHeaders.size())
        {
            if(m_vHeaders[indexHeader]      == "[ VERSION ]"){ m_outFile << m_vHeaders[indexHeader] << "\n" << m_sVersion << std::endl; }
            else if(m_vHeaders[indexHeader] == "[ LOG FOLDER ]") { m_outFile << m_vHeaders[indexHeader] << "\n" << m_sLogFolder << std::endl; }
            else if(m_vHeaders[indexHeader] == "[ LOG FILE ]") { m_outFile << m_vHeaders[indexHeader] << "\n" << m_sLogFile << std::endl; }
            else if(m_vHeaders[indexHeader] == "[ MAX THREADS ]") { m_outFile << m_vHeaders[indexHeader] << "\n" << m_sMaxThreads <<"\n" <<"-EOS-" <<"\n" << std::endl; }
            else if(m_vHeaders[indexHeader] == "[ MIN CODE ]") { m_outFile << m_vHeaders[indexHeader] << "\n" << m_sMinCode << std::endl; }
            else if(m_vHeaders[indexHeader] == "[ MAX CODE ]") { m_outFile << m_vHeaders[indexHeader] << "\n" << m_sMaxCode << std::endl; }
            else if(m_vHeaders[indexHeader] == "[ RESOLUTION ]") { m_outFile << m_vHeaders[indexHeader] << "\n" << m_sResolution << std::endl; }
            else if(m_vHeaders[indexHeader] == "[ RETRIEVE TYPE ") { m_outFile << m_vHeaders[indexHeader] << "\n" << m_sRetrieveType << std::endl; }
            else if(m_vHeaders[indexHeader] == "[ COVERAGE MAP ALGORITHM ]") { m_outFile << m_vHeaders[indexHeader] << "\n" << m_sCoverageMapAlgorithm << std::endl; }
            else if(m_vHeaders[indexHeader] == "[ IGNORE TX WITH MISSING LOSSES ]") { m_outFile << m_vHeaders[indexHeader] << "\n" << m_sIgnoreTxWithMissingLosses << "\n" << "-EOS-" <<"\n" << std::endl; }
            else if(m_vHeaders[indexHeader] == "[ MAX RADIUS UNIT ]") { m_outFile << m_vHeaders[indexHeader] << "\n" << m_sMaxRadiusUnit << std::endl; }
            else if(m_vHeaders[indexHeader] == "[ BASIC PARAMETERS ]") { m_outFile << m_vHeaders[indexHeader] << "\n" << m_sBasicParameters << "\n" << "-EOS-" << "\n" << "-EOF" <<"\n" << std::endl; }
            else{ m_outFile << m_vHeaders[indexHeader] << std::endl;}
            indexHeader++;
        }
        m_outFile.close();
    }
    if (m_bFlag == true) { m_IniObj.writeLogFile("Validation completed", 1, 0); }
    else { m_IniObj.writeLogFile("Validation failed", 1, 0); }
    //return the flag
    return m_bFlag;

}