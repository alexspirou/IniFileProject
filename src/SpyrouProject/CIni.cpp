#include "CIni.h"
#include <algorithm>

CIni::CIni() 
{
	std::cout << "CIni constructor called " << std::endl;
    readFile();
    std::string* s = new std::string();
    s = NULL;
    writeFile(s);
    validateFile();
}

void CIni::readFile() 
{
    int headersIndex = { 0 };
    //count header vector size
    inFile.open(sFileName);
    while (getline(inFile, m_sNextLine))
    {
        if (m_sNextLine.find(']') != std::string::npos) {
            headerSize++;
        }
    }
    inFile.close();

    m_vHeaders.resize(headerSize);
    inFile.open(sFileName);
    if (!inFile) {
        std::cerr << "error: not open" << std::endl;
    }
    //read file//

    while (getline(inFile, m_sNextLine))
    {
        if (m_sNextLine.find('[') != std::string::npos) {
            m_vHeaders[headersIndex] = (m_sNextLine);
            headersIndex++;
        }
    }
    inFile.close();
}

void CIni::writeFile(std::string* userInput)
{
    //Indices for vectors
    int j{ 0 };
    int i{ 0 };

    vHeaderValues.resize(m_vHeaders.size() - m_vSectionNames.size());

    vHeaderValues[0] = "2.7.8"; // [VERSION]
    vHeaderValues[1] = "--";    //[LOG FOLDER]
    vHeaderValues[2] = "foder";  //[LOG FILE]
    vHeaderValues[3] = "1";      //[MAX THREADS]
    vHeaderValues[4] = "-1"; //[MIN CODE]
    vHeaderValues[5] = "-1"; //[MAX CODE]
    vHeaderValues[6] = "100"; //[RESOLUTION]
    vHeaderValues[7] = "DISTANCE"; //[RETRIEVE TYPE]
    vHeaderValues[8] = "BASIC"; //[COVERAGE MAP ALGORITHM]
    vHeaderValues[9] = "YES"; //[IGNORE TX WITH MISSING LOSSES]
    vHeaderValues[10] = "km"; //[MAX RADIUS UNIT]
    vHeaderValues[11] = "-106.0,100,150.0,10000.0,1,9.0,1,-98.0,2.0,-98.0,3.0,1,0,0"; //[BASIC PARAMETERS]

    auto it = find(m_vHeaders.begin(), m_vHeaders.end(), m_vSectionNames[0]);
    if (it != m_vHeaders.end())
    {
        indexSectionA = it - m_vHeaders.begin();
    }
    it = find(m_vHeaders.begin(), m_vHeaders.end(), m_vSectionNames[1]);
    if (it != m_vHeaders.end()) 
    {
        indexSectionB = it - m_vHeaders.begin();
    }

    outFile.open(sFileName, std::ofstream::trunc);

    while (outFile && i < m_vHeaders.size()) 
    {
        if (userInput == NULL) {
            outFile << m_vHeaders[i] << "\n" << vHeaderValues[j] << std::endl;
            if (i == indexSectionA - 1)
            {
                i++;
                outFile << "-EOS-\n" << "\n" << m_vHeaders[i] << std::endl;
            }
            if (i == indexSectionB - 1)
            {
                i++;
                outFile << "-EOS-\n" << "\n" << m_vHeaders[i] << std::endl;
            }
            i++; j++;
        }
        else {
            outFile << m_vHeaders[i] << "\n" << vHeaderValues[j] << std::endl;
            std::cout << "here" << std::endl;
        }
    }

    outFile << "-EOS-" << "\n" << "-EOF-" << std::endl;

    outFile.close();
}

void CIni::validateFile()
{
    //Validate
    auto toInt = [](std::string s)
    {
        int temp = stoi(s);
        return temp;
    };
    auto toUpper = [](std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    };
    //Indices for vectors
    int headersIndex = 0;
    int validationsIndex = 0;

    int m_IEosCounter = 0;
    m_vHeaders.erase(m_vHeaders.begin() + indexSectionB);
    m_vHeaders.erase(m_vHeaders.begin() + indexSectionA);
   
    inFile.open(sFileName);
    m_vValData.resize(m_vHeaders.size());

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
        if (m_sNextLine == m_vHeaders[headersIndex] && m_sNextLine != "[ BASIC PARAMETERS ]") 
        {
            getline(inFile, m_sNextLine);
            m_vValData[validationsIndex] = m_sNextLine;
            validationsIndex++;
            headersIndex++;
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

    bool m_vValidationCheck = true;

    //[VERSION]
    m_vValidationCheck = (m_vValData[0] == "2.7.8") ? 1 : 0;
    //[LOG FOLDER]
    m_vValidationCheck = (m_vValData[1] == "--") ? 1 : 0;
    //[ LOG FILE ]
    m_vValidationCheck = (m_vValData[2] == "foder") ? 1 : 0;
    //[ MAX THREADS ]
    m_vValidationCheck = (toInt(m_vValData[3]) >= 0 && toInt(m_vValData[3]) <= m_vMaxThreads) ? 1 : 0;
    //[ MIN CODE ]
    m_vValidationCheck = (toInt(m_vValData[4]) >= m_vCodeLimits[0] && (toInt(m_vValData[4]) <= m_vCodeLimits[1])) ? 1 : 0;
    //[ MAX CODE ]
    m_vValidationCheck = (toInt(m_vValData[5]) >= m_vCodeLimits[0] && toInt(m_vValData[5]) <= m_vCodeLimits[1]) ? 1 : 0;
    //[ RESOLUTION ]
    m_vValidationCheck = (toInt(m_vValData[6]) >= (m_vResolutionLimits[0]) && toInt(m_vValData[6]) <= (m_vResolutionLimits[1])) ? 1 : 0;
    //[RETRIEVE TYPE]
    m_vValidationCheck = (toUpper(m_vValData[7]) == (toUpper(m_vRetrieveType[0])) || toUpper(m_vValData[7]) == toUpper(m_vRetrieveType[1])) ? 1 : 0;
    //[COVERAGE MAP ALGORITHM]
    m_vValidationCheck = (toUpper(m_vValData[8]) == toUpper(m_vCoverMapAlgorithm[0]) || toUpper(m_vValData[8]) == toUpper(m_vCoverMapAlgorithm[1])) ? 1 : 0;
    //[IGNORE TX WITH MISSING LOSSES]
    m_vValidationCheck = (toUpper(m_vValData[9]) == toUpper(m_vIgnoreMissignLoses[0]) || toUpper(m_vValData[9]) == toUpper(m_vIgnoreMissignLoses[1])) ? 1 : 0;
    //[ MAX RADIUS UNIT ]
    m_vValidationCheck = (toUpper(m_vValData[10]) == toUpper(m_vMaxRadiusUnits[0]) || toUpper(m_vValData[10]) == toUpper(m_vMaxRadiusUnits[1])) ? 1 : 0;

}