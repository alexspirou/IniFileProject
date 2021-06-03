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


    std::ifstream inFile;
    std::ofstream outFile;
    int i = 0;
    int j = 0;
    size_t headerSize = 0;
    std::string nextLine{};

    //count header vector size
    inFile.open("Ini.ini");
    while (getline(inFile, nextLine))
    {
        headerSize ++;
    }
    inFile.close();

    std::vector<std::string> vSHeaders(headerSize);
    inFile.open("Ini.ini");
    if (!inFile) {
        std::cerr << "error: not open" << std::endl;
    }
    //read file//

    while (getline(inFile, nextLine)) 
    {
        if (nextLine.find('[') != std::string::npos) {
            vSHeaders[i] = (nextLine);
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
    auto it = find(vSHeaders.begin(), vSHeaders.end(), "[ -A SECTION- ]");
    if (it != vSHeaders.end()) {
          indexSectionA = it - vSHeaders.begin();
    }
    it = find(vSHeaders.begin(), vSHeaders.end(), "[ -B SECTION- ]");
    if (it != vSHeaders.end()) {
          indexSectionB = it - vSHeaders.begin();
    }


    outFile.open("Ini.ini", std::ofstream::trunc);

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

    inFile.open("Ini.ini");
    std::vector<std::string> vSValidatedData{20};
    i = 0;
    int counter = 0;
    while (getline(inFile, nextLine) && i < vSValidatedData.size()) {
        getline(inFile, nextLine);
        if (nextLine == "-EOS-") {
            counter++;
        }
        if (counter == 0) {
            vSValidatedData[i] = nextLine;
        }
        else if (counter == 1) {
            
            getline(inFile, nextLine);

            vSValidatedData[i] = nextLine;
        }
        else if (counter == 2) {
            getline(inFile, nextLine);
            vSValidatedData[i] = nextLine;
        }
       
        i ++;
    }
    print(vSValidatedData);
    inFile.close();
}