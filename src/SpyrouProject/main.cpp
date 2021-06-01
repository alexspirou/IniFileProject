#include "CIni.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

template <typename T>
void print(std::vector<T> arr) {
    for (auto a : arr) {
        std::cout << a << " ";
    }
    std::cout << std::endl;
}


int main()
{

    std::fstream file;
    file.open( "Ini.ini" ,std::ios::in | std::ios::out);


   
    

    if (!file) {
        std::cerr << "error: not open" << std::endl;
    }
    std::string a{ "a" };
    std::string nextSpace;
    double nextDouble{};
    

    while (file >> nextSpace) {
        
        if (nextSpace == "VERSION")
        {
            file >> nextSpace;
            file.seekp(file.tellp());
            file  << "2fefwewfew" << std::endl;
            file >> nextSpace;
         }

        else if (nextSpace == "FOLDER")
        {
            file >> nextSpace;
            int  position = file.tellp();
            file.seekp(position);
           // file << "\n" << " \n" << std::endl;
           // file >> nextSpace;
        }
    }
    file.close();
  
    file.open("Ini.ini", std::ios::in | std::ios::out);
    while (file >> nextSpace) {

        if (nextSpace == "VERSION") {
            std::getline(file, nextSpace);
            file >> nextSpace;
            std::cout << nextSpace << std::endl;

        }
    }

    file.close();

}