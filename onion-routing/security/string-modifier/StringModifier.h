//
// Created by xray2 on 18/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_STRINGMODIFIER_H
#define IDATT2104___PROJECT___ONION_ROUTER_STRINGMODIFIER_H


#include <string>
#include "vector"
#include "../model/Cryption.h"
/**
 * Class used for modifying strings and Cryption (custom class)
 */
class StringModifier {
public:
    static std::vector<std::string> splitString(std::string string, int splitLength);
    static std::string cryptionToString(Cryption &cryption);

    static string BN2Str(BIGNUM *num);

    static long long int BN2LLI(BIGNUM *num);

    static Cryption splitString(string string);

    static vector<int> getVector(int num, int size, int offset);


    static unsigned char *convertToCharArray(string text);
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_STRINGMODIFIER_H
