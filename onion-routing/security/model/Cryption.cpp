//
// Created by xray2 on 18/03/2022.
//

#include "Cryption.h"

const vector <vector<char>> &Cryption::getStrings() const {
    return strings;
}

const vector<int> &Cryption::getStringsLen() const {
    return strings_len;
}

Cryption::Cryption(vector<vector<char>> strings, vector<int> stringsLen) {
    strings = strings;
    strings_len = stringsLen;
}

Cryption::Cryption(vector<vector<char>> vector1) {
    strings = vector1;
}

Cryption::Cryption() {

}

Cryption::Cryption(vector<unsigned char*> vector1, vector<int> stringsLen) {
    res = vector1;
    strings_len = stringsLen;
}

const vector<unsigned char *> Cryption::getRes() const {
    return res;
}


