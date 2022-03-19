//
// Created by xray2 on 18/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_STRINGMODIFIER_H
#define IDATT2104___PROJECT___ONION_ROUTER_STRINGMODIFIER_H


#include <string>
#include "vector"
class StringModifier {
public:
    static unsigned char* convertToCharArray(long long int a);
    static std::vector<std::string> splitString(std::string string, int splitLength);
    static unsigned char* convertToCharArray(std::string text);
    static std::vector<char> stringToCharVector(std::string str);
    static std::vector<std::vector<char>> stringToCharArray(std::string str);

    const unsigned char *convert(const std::string &s);
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_STRINGMODIFIER_H
