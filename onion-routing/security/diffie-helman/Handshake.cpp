//
// Created by xray2 on 19/03/2022.
//
#include "Handshake.h"
#include <stdexcept>
bool Handshake::doHandshake(Node n1, Node n2) {
//    try {
//        unsigned long long int secretKey1 = n1.getKey.getSecretKey(bobKey.generateKey());
//        long long int secretKey = secretKey1 / 10000;
//    }catch (int num){
//        return false;
//    }
}

unsigned long long int Handshake::doHandshake(Key &k1, Key &k2) {
    unsigned long long int secretKeyk1 = k1.getSecretKey(k2.generateKey());
    unsigned long long int secretKeyk2 = k2.getSecretKey(k1.generateKey());
    if(secretKeyk1 != secretKeyk2)
        throw std::invalid_argument("Handshake error");


    return secretKeyk1;
}
