#include "CIni.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
template <typename T>
void print(std::vector<T> arr) {
    for (auto a : arr) {
        std::cout << a << " ";
    }
    std::cout << std::endl;
}


int main()
{

    std::string a{ "a" };
    std::string wholestring;
    double nextDouble{};

    const char* msg = "\nalex";

    std::fstream file;
    file.open("Ini.ini", std::ios::in | std::ios::out);
    char c;
 
    if (!file) {
        std::cerr << "error: not open" << std::endl;
    }
    std::string in, out;
    int blockNumber = 0;//Which bracket block we are looking for. We are currently looking for the second one.

    while (file.get(c)) {

        wholestring.push_back(c);
            
     }
    file.close();
    int counter = 0;
    std::string check{};
    for (int i{ 0 }; i < wholestring.length(); i++) 
    {
        check.push_back(wholestring[i]);
        if (check == "[ VERSION ]") 
        {
            std::string temp = "\n1.5.7";
            wholestring.replace(i + 1, 0, temp);
            i += temp.length()+1;
            check.clear();
        }
        if (check == "[ LOG FOLDER ]") 
        {
            std::string temp = "\n--";
            wholestring.replace(i + 1, 0, temp);
            i += temp.length() + 1;
            check.erase();
        }
        if (check == "[ LOG FILE ]") 
        {
            std::string temp = "\n C:\\Users\\Alex\\Desktop  \\test\\  test_project\\test_project";

            temp.erase(remove_if(temp.begin(), temp.end(), isspace), temp.end());
            wholestring.replace(i + 1, 0, temp);
            i += temp.length() + 1;
            check.erase();
        }
        if (check == "[ MAX THREADS ]") 
        {
            std::string temp = "\n 1";
            temp.erase(remove_if(temp.begin(), temp.end(), isspace), temp.end());
            wholestring.replace(i + 1, 0, temp);
            i += temp.length() + 1;
            check.erase();
        }
        if (check == "-EOS-") {
            counter++;
            check.erase();
        }
    }
    std::cout << counter << std::endl;
}