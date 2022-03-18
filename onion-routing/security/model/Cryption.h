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
    vector <vector<char>> strings;
    vector<int> strings_len;
    vector <unsigned char*> res;
public:
    Cryption(vector <vector<char>> strings, vector<int> stringsLen);

    //Cryption(const vector <string> &strings);

    const vector <vector<char>> &getStrings() const;

    const vector<int> &getStringsLen() const;

    Cryption(vector <vector<char>> vector1);

    Cryption(vector <unsigned char*>  vector1, vector<int> stringsLen);

    Cryption();

    const vector<unsigned char *> getRes() const;
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_CRYPTION_H
