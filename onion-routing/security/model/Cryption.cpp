//
// Created by xray2 on 18/03/2022.
//

#include "Cryption.h"

/**
 * Getter
 * @return
 */
const vector<int> &Cryption::getStringsLen() const {
    return strings_len;
}

/**
 * Constructor
 * @param strings
 * @param stringsLen
 */
Cryption::Cryption(vector<unsigned char *> strings, vector<int> stringsLen) {
    strings = strings;
    strings_len = stringsLen;
}

/**
 * Getter
 * @return
 */
Cryption::Cryption() {

}

/**
 * Setter
 * @param stringsLen
 */
vector<unsigned char *> Cryption::getRes(){
    return res;
}

/**
 * Setter
 * @param stringsLen
 */
void Cryption::setStringsLen(const vector<int> &stringsLen) {
    strings_len = stringsLen;
}

/**
 * Setter
 * @param res
 */
void Cryption::setRes(const vector<unsigned char *> &res) {
    Cryption::res = res;
}

/**
 * Returns a string with info about how many blocks, block size and offset
 * @return
 */
string Cryption::getRequestString(){
    int len = strings_len.size();
    return to_string(len) + "|" + to_string(strings_len[0]) + "|" + to_string(strings_len[len-1]);
}
