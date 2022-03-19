//
// Created by xray2 on 18/03/2022.
//

#include <cmath>
#include <cstring>
#include "StringModifier.h"

unsigned char* StringModifier::convertToCharArray(long long int a)
{
    unsigned char* arr;
    int i = 0;

    for (i = 0; i < 8; ++i)
    {
        arr[i] = (unsigned char)((((unsigned long long) a) >> (56 - (8*i))) & 0xFFu);
    }
    return arr;
}

std::vector<std::string> StringModifier::splitString(std::string string, int splitLength) {
    std::vector<std::string> vector;
    int length = string.length();
    int intervals = length/splitLength;

    for (auto i = 0; i < intervals; i++)
    {
        vector.push_back(string.substr(i * splitLength, splitLength));
    }

    if (string.length() % splitLength != 0)
    {
        vector.push_back(string.substr(splitLength * intervals));
    }

    return vector;
}

unsigned char *StringModifier::convertToCharArray(std::string text) {
    return (unsigned char*) text.c_str();
}


std::string StringModifier::cryptionToString(Cryption &cryption) {

    string returnString;
    for(int i = 0; i < cryption.getRes().size(); i++){
        for(int y = 0; y < cryption.getStringsLen()[i]; y++){
            returnString += cryption.getRes()[i][y];
        }
    }
    return returnString;
}
