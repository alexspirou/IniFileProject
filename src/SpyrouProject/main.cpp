#include "CIni.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
template <typename T>
void print(std::vector<T> arr) {
    for (auto a : arr) {
        std::cout << a << std::endl;
    }
    std::cout << std::endl;
}


int main()
{
    int aa = 10;
    int vv = 20;

    int var =  (10 < 20 && 1 < 2 && 200 < 20) ? 50 : 20;

    std::ifstream inFile{};
    std::ofstream outFile{};
    std::string sFileName{ "Application.ini" };
    std::vector<std::string> m_vSectionNames{ "[ -A SECTION- ]" , "[ -B SECTION- ]" };

    int i = 0;
    int j = 0;
    size_t headerSize = 0;
    std::string m_sNextLine{};

    //count header vector size
    inFile.open(sFileName);
    while (getline(inFile, m_sNextLine))
    {
        if (m_sNextLine.find(']') != std::string::npos) {
            headerSize++;
        }
    }
    inFile.close();

    std::vector<std::string> vSHeaders(headerSize);
    inFile.open(sFileName);
    if (!inFile) {
        std::cerr << "error: not open" << std::endl;
    }
    //read file//

    while (getline(inFile, m_sNextLine)) 
    {
        if (m_sNextLine.find('[') != std::string::npos) {
            vSHeaders[i] = (m_sNextLine);
            i++;
        }
    }
    inFile.close();

    //write file
    std::vector<std::string> vHeaderValues{ 20 };
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

    int indexSectionA{0};
    int indexSectionB{0};
    auto it = find(vSHeaders.begin(), vSHeaders.end(), m_vSectionNames[0]);
    if (it != vSHeaders.end()) {
          indexSectionA = it - vSHeaders.begin();
    }
    it = find(vSHeaders.begin(), vSHeaders.end(), m_vSectionNames[1]);
    if (it != vSHeaders.end()) {
          indexSectionB = it - vSHeaders.begin();
    }


    outFile.open(sFileName, std::ofstream::trunc);

    i = 0;
    while (outFile && i < vSHeaders.size()) {

        outFile << vSHeaders[i] << "\n" << vHeaderValues[j] << std::endl;
        if (i == indexSectionA -1)
        {
            i++;
          
            outFile << "-EOS-\n" << "\n"<< vSHeaders[i] << std::endl;
        }
        if (i == indexSectionB - 1)
        {    
            i++;
            outFile << "-EOS-\n" << "\n" << vSHeaders[i] << std::endl;
        }
        i++; j++;
    }

    outFile << "-EOS-" << "\n" << "-EOF-" << std::endl;

    outFile.close();
    

    //Validate

    inFile.open(sFileName);

    std::vector<std::string> vSValidatedData{ headerSize };
    i = 0;
    int m_IEosCounter = 0;

    while (getline(inFile, m_sNextLine )&& m_sNextLine!="-EOF-") {

        getline(inFile, m_sNextLine);

        (m_sNextLine == "-EOS-") ? getline(inFile, m_sNextLine) : inFile;
        (m_sNextLine == "") ? getline(inFile, m_sNextLine) : inFile;
        (m_sNextLine == m_vSectionNames[0] || m_sNextLine == m_vSectionNames[1]) ? inFile >> m_sNextLine : inFile;
        if (m_sNextLine == "-EOF-")
        {
            break;
        }
        vSValidatedData[i] = m_sNextLine;
        i++;
        
    }
    std::cout << vSValidatedData.size() << std::endl;
    print(vSValidatedData);
    std::cout << vSValidatedData.size();
    inFile.close();
}

