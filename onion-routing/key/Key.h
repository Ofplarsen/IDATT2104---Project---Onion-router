//
// Created by xray2 on 14/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_KEY_H
#define IDATT2104___PROJECT___ONION_ROUTER_KEY_H


class Key {
private:
    long long int privateKey;
    long long int publicKeyP;
    long long int publicKeyG;
    long long int generatedKey;
    long long int secretKey;


public:
    long long int generateKey();
    long long int getSecretKey();
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_KEY_H
