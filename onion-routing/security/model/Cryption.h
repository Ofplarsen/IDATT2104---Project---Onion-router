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

public:
    vector<int> strings_len;
    vector <unsigned char*> res;

    //Cryption(const vector <string> &strings);

    const vector<int> &getStringsLen() const;


    Cryption(vector <unsigned char*>  vector1, vector<int> stringsLen);

    Cryption();

    vector<unsigned char *> getRes();


    void setStringsLen(const vector<int> &stringsLen);

    void setRes(const vector<unsigned char *> &res);
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_CRYPTION_H
