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

    std::ifstream inFile{};
    std::ofstream outFile{};
    std::string sFileName{ "Application.ini" };
    std::vector<std::string> m_vSectionNames{ "[ -A SECTION- ]" , "[ -B SECTION- ]" };

    //indeces
    int headersIndex = { 0 };
    int valuesIndex{ 0 };
    int validationsIndex{ 0 };


    size_t headerSize{ 0 };
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

    std::vector<std::string> m_vHeaders(headerSize);
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
    auto it = find(m_vHeaders.begin(), m_vHeaders.end(), m_vSectionNames[0]);
    if (it != m_vHeaders.end()) {
          indexSectionA = it - m_vHeaders.begin();
    }
    it = find(m_vHeaders.begin(), m_vHeaders.end(), m_vSectionNames[1]);
    if (it != m_vHeaders.end()) {
          indexSectionB = it - m_vHeaders.begin();
    }


    outFile.open(sFileName, std::ofstream::trunc);

    headersIndex = 0;
    while (outFile && headersIndex < m_vHeaders.size()) {

        outFile << m_vHeaders[headersIndex] << "\n" << vHeaderValues[valuesIndex] << std::endl;
        if (headersIndex == indexSectionA -1)
        {
            headersIndex++;
          
            outFile << "-EOS-\n" << "\n"<< m_vHeaders[headersIndex] << std::endl;
        }
        if (headersIndex == indexSectionB - 1)
        {    
            headersIndex++;
            outFile << "-EOS-\n" << "\n" << m_vHeaders[headersIndex] << std::endl;
        }
        headersIndex++; valuesIndex++;
    }

    outFile << "-EOS-" << "\n" << "-EOF-" << std::endl;

    outFile.close();
    

    //Validate

    inFile.open(sFileName);
    m_vHeaders.erase(m_vHeaders.begin() + indexSectionB);
    m_vHeaders.erase(m_vHeaders.begin()+ indexSectionA);
    std::vector<std::string> vSValidatedData{ m_vHeaders.size() };
    std::vector<double> doubleData{ 20};
    double nextDouble;
    headersIndex = 0;
    int i = 0;
    char c{};
    int m_IEosCounter = 0;
    while (getline(inFile, m_sNextLine)) {

        
        m_IEosCounter = (m_sNextLine == "-EOS-") ? m_IEosCounter += 1 : m_IEosCounter;
        if (m_IEosCounter >= 3) {
            break;
        }
        //Change line when the header has the same value and store the data
        if (m_sNextLine == m_vHeaders[headersIndex] && m_sNextLine!= "[ BASIC PARAMETERS ]") {
            getline(inFile, m_sNextLine);
            vSValidatedData[validationsIndex] = m_sNextLine;
            validationsIndex ++;
            headersIndex++;
        }
    }
    inFile.close();

    inFile.open(sFileName);

    while (inFile >>nextDouble){
        m_IEosCounter = (m_sNextLine == "-EOS-") ? m_IEosCounter += 1 : m_IEosCounter;
        if (m_IEosCounter < 2) {
            getline(inFile, m_sNextLine);

        }
        if (m_IEosCounter == 2) {
            doubleData[i] = nextDouble;
            i++;
        }

    }



    inFile.close();

    print(vSValidatedData);
    print(doubleData);


}

