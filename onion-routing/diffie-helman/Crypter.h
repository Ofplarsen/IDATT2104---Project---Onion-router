//
// Created by xray2 on 16/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_CRYPTER_H
#define IDATT2104___PROJECT___ONION_ROUTER_CRYPTER_H


class Crypter {
public:
    static int decrypt(int message, int key, int publicKey);
    static int encrypt(int message, int key, int publicKey);

    static int modInverse(int a, int m);
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_CRYPTER_H
