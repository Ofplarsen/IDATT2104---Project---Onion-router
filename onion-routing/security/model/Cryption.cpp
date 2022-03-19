//
// Created by xray2 on 18/03/2022.
//

#include "Cryption.h"

const vector<int> &Cryption::getStringsLen() const {
    return strings_len;
}

Cryption::Cryption(vector<unsigned char *> strings, vector<int> stringsLen) {
    strings = strings;
    strings_len = stringsLen;
}


Cryption::Cryption() {

}

vector<unsigned char *> Cryption::getRes(){
    return res;
}


void Cryption::setStringsLen(const vector<int> &stringsLen) {
    strings_len = stringsLen;
}

void Cryption::setRes(const vector<unsigned char *> &res) {
    Cryption::res = res;
}
