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
    std::vector<std::string> vSHeaders(14);
    std::string nextLine{};
    inFile.open("Ini.ini");
    if (!inFile) {
        std::cerr << "error: not open" << std::endl;
    }
    int blockNumber = 0;//Which bracket block we are looking for. We are currently looking for the second one.
    //read file
    int i = 0;
    int j = 0;
    while (getline(inFile, nextLine)) 
    {
        vSHeaders[i] = (nextLine);
        i++;
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
   
    //std::string s;
    //int counter = 0;
    //for (i = 0, j=0; i < vSHeaders.size() && j < vSHeaders.size(); i++,j++)
    //{
    //    if (vSHeaders[i] == "[ -A SECTION- ]") {

    //        vSHeaders[i].replace(0, 0, "-EOS-\n\n");
    //        i++;
    //        
    //    }
    //    if (vSHeaders[i] == "[ -B SECTION- ]") {

    //        vSHeaders[i].replace(0, 0, "-EOS-\n\n");
    //        i++;
    //       
    //    }
    //    
    //    vSHeaders[i].replace(vSHeaders[i].length(), 0, "\n" + vHeaderValues[j]);
    //    //std::cout << vSHeaders[i] << std::endl;
    //    //getline(std::cin, s);
    //    //std::cout << std::endl;

    //    //vSHeaders[i].replace(vSHeaders[i].length(), 0, "\n" + s);
    //    //input

    //}
    //vSHeaders[vSHeaders.size()-1].replace(vSHeaders[vSHeaders.size()-1].length(), 0, "\n-EOS-\n-EOF-");
    
    outFile.open("Ini.ini", std::ofstream::trunc);
    i = 0;
    std::streampos  pos;
    while (outFile && i < vSHeaders.size()) {
       pos = outFile.tellp();

        if (vSHeaders[i] == "[ -A SECTION- ]")
        {
            std::streampos temPos = pos;
            outFile.seekp(pos);
            outFile << "-EOS-" << std::endl;
            outFile.seekp(temPos);
        }
        if (vSHeaders[i] == "[ -B SECTION- ]")
        {
            std::streampos temPos = outFile.tellp();
            outFile.seekp(pos);
            outFile << "-EOS-" << std::endl;
            outFile.seekp(temPos);

        }
        outFile << vSHeaders[i] << "\n" << vHeaderValues[i] << "\n";
        std::cout << pos << std::endl;


        i++;
    }
    print(vSHeaders);


     
        

 
}