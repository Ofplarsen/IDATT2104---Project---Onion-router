//
// Created by xray2 on 18/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_STRINGMODIFIER_H
#define IDATT2104___PROJECT___ONION_ROUTER_STRINGMODIFIER_H


#include <string>
#include "vector"
#include "../model/Cryption.h"
/**
 * Class used for modifying strings
 */
class StringModifier {
public:
    static unsigned char* convertToCharArray(long long int a);
    static std::vector<std::string> splitString(std::string string, int splitLength);
    static unsigned char* convertToCharArray(std::string text);
    static std::string cryptionToString(Cryption &cryption);
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_STRINGMODIFIER_H
