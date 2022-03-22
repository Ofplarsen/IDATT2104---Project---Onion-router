//
// Created by xray2 on 18/03/2022.
//

#include <cmath>
#include <cstring>
#include <openssl/bn.h>
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

Cryption StringModifier::splitString(std::string string) {
    Cryption c;
    std::vector<unsigned char*> vector;
    std::vector<int> lengths;
    int length = string.length();
    int intervals = length/16;


    for (auto i = 0; i < intervals; i++)
    {
        c.res.push_back(convertToCharArray(string.substr(i * 16, 16)));
        c.strings_len.push_back(16);
    }

    if (string.length() % 16 != 0)
    {
        int l = string.substr(16 * intervals).length();
        c.res.push_back(convertToCharArray(string.substr(16 * intervals)));
        c.strings_len.push_back(l);
    }

    return c;
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

long long int StringModifier::BN2LLI(BIGNUM *num){
    BIGNUM *dupNum;
    dupNum = BN_dup(num);

    char* charNum;

    charNum = BN_bn2dec(dupNum);
    string strNum = string(charNum);
    return stoll(strNum, nullptr, 10);
}

string StringModifier::BN2Str(BIGNUM *num){
    BIGNUM *dupNum;
    dupNum = BN_dup(num);

    char* charNum;

    charNum = BN_bn2dec(dupNum);

    return string(charNum);
}