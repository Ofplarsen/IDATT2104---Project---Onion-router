//
// Created by xray2 on 18/03/2022.
//

#include "Cryption.h"


const vector<int> &Cryption::getStringsLen() const {
    return strings_len;
}

Cryption::Cryption(vector<unsigned char *> strings, vector<int> stringsLen) {
    res = strings;
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

string Cryption::getRequestString(){
    int len = strings_len.size();
    return to_string(len) + "|" + to_string(strings_len[0]) + "|" + to_string(strings_len[len-1])+"\r\n";
}

int Cryption::getLength() {
    int l = (strings_len.size()-1)*32;
    l += strings_len[strings_len.size()-1];
    return l;
}
