//
// Created by xray2 on 18/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_CRYPTION_H
#define IDATT2104___PROJECT___ONION_ROUTER_CRYPTION_H


#include <vector>
#include <string>
using namespace std;
class Cryption {
private:
    vector<string> strings;
    vector<int> strings_len;
public:
    Cryption(vector <string> strings, vector<int> stringsLen);

    //Cryption(const vector <string> &strings);

    const vector <string> &getStrings() const;

    const vector<int> &getStringsLen() const;

    Cryption(vector<string> vector1);
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_CRYPTION_H
