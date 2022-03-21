//
// Created by xray2 on 19/03/2022.
//
#include "Handshake.h"
#include <stdexcept>
#include <iostream>

bool Handshake::doHandshake(Node n1, Node n2) {
//    try {
//        unsigned long long int secretKey1 = n1.getKey.getSecretKey(bobKey.generateKey());
//        long long int secretKey = secretKey1 / 10000;
//    }catch (int num){
//        return false;
//    }
}

BIGNUM * Handshake::doHandshake(Key &k1, Key &k2) {
    BIGNUM *secretKeyk1 = BN_new();
    secretKeyk1 = k1.getSecretKey(k2.generateKey());
    BIGNUM *secretKeyk2 = BN_new();
    secretKeyk2 = k2.getSecretKey(k1.generateKey());
    if(BN_cmp(secretKeyk1, secretKeyk2) == 1)
        throw std::invalid_argument("Handshake error");

    cout << ("6*7=%s\n", BN_bn2dec(secretKeyk1)) << endl;
    cout << ("6*7=%s\n", BN_bn2dec(secretKeyk2)) << endl;

    return secretKeyk1;
}
