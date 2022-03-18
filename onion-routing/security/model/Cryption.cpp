//
// Created by xray2 on 18/03/2022.
//

#include "Cryption.h"

const vector <std::string> &Cryption::getStrings() const {
    return strings;
}

const vector<int> &Cryption::getStringsLen() const {
    return strings_len;
}

Cryption::Cryption(std::vector<std::string> strings, vector<int> stringsLen) {
    strings = strings;
    strings_len = stringsLen;
}

Cryption::Cryption(std::vector<std::string> vector1) {
    strings = vector1;
}

