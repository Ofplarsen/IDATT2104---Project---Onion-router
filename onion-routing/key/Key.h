//
// Created by xray2 on 14/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_KEY_H
#define IDATT2104___PROJECT___ONION_ROUTER_KEY_H


class Key {
private:
    long long int privateKey;
    long long int publicKey1;
    long long int publicKey2;
    long long int generatedKey;
    long long int secretKey;



    long long int generateKey();
    long long int getSecretKey();
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_KEY_H
