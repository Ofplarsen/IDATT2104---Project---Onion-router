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
    int intervals = length/32;

    for (auto i = 0; i < intervals; i++)
    {
        unsigned char decrypted[32];
        memset(decrypted, 0, 32);

        const std::string text = (string.substr(i * 32, 32));

        for (int j = 0; j < 32; ++j) {
            decrypted[j] = text.c_str()[j];
        }

        char * copy = static_cast<char *>(malloc(32));
        strcpy(copy, reinterpret_cast<const char *>(decrypted));

        c.strings_len.push_back(32);
        c.res.emplace_back(reinterpret_cast<unsigned char *const>(copy));
    }

    if (string.length() % 32 != 0)
    {
        int l = string.substr(32 * intervals).length();
        std::string text = (string.substr(intervals * 32, l));
        unsigned char decrypted[l];
        memset(decrypted, 0, l);
        for (int j = 0; j < l; ++j) {
            decrypted[j] = text.c_str()[j];
        }

        char * copy = static_cast<char *>(malloc(l));
        strcpy(copy, reinterpret_cast<const char *>(decrypted));

        c.strings_len.push_back(l);
        c.res.emplace_back(reinterpret_cast<unsigned char *const>(copy));
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
    BIGNUM *dupNum = BN_new();
    BN_copy(dupNum,num);

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

int StringModifier::getLengthOfLastBlock(string text){
    int length = text.length();
    int intervals = length/32;

    if (text.length() % 32 != 0)
    {
        return text.substr(32 * intervals).length();
    }
    return 32;
}

vector<int> StringModifier::getVector(int num, int size, int offset){
    vector<int> v;
    for (int i = 0; i < num-1; ++i) {
        v.push_back(size);
    }
    v.push_back(offset);
    return v;
}