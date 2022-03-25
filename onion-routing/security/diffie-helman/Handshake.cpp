//
// Created by xray2 on 19/03/2022.
//
#include "Handshake.h"
#include <stdexcept>
#include <iostream>
#include "../string-modifier/StringModifier.h"
bool Handshake::doHandshake(Node n1, Node n2) {
//    try {
//        unsigned long long int secretKey1 = n1.getKey.getSecretKey(bobKey.generateKey());
//        long long int secretKey = secretKey1 / 10000;
//    }catch (int num){
//        return false;
//    }
}

BIGNUM * Handshake::doHandshake(Key &k1, Key &k2) {
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *secretKeyk1 = BN_new();
    secretKeyk1 = k1.getSecretKey(k2.generateKey());
    BIGNUM *secretKeyk2 = BN_new();
    secretKeyk2 = k2.getSecretKey(k1.generateKey());
    if(BN_cmp(secretKeyk1, secretKeyk2) == 1){
        BN_CTX_free(ctx);
        throw std::invalid_argument("Handshake error");
    }
    int l = StringModifier::BN2Str(secretKeyk1).length();
    if(l < 16){
        BIGNUM *ten = BN_new();
        const char* c = "10";
        BN_dec2bn(&ten, c);
        for(int i = l; i < 16; i++){
            BN_mul(secretKeyk1, secretKeyk1, ten, ctx);
            BN_mul(secretKeyk2, secretKeyk2, ten, ctx);
        }
        BN_clear_free(ten);

    }
    if(l > 16){
        BIGNUM *ten = BN_new();
        BIGNUM *rem = BN_new();
        const char* c = "10";
        BN_dec2bn(&ten, c);
        for(int i = l; i > 16; i--){
            BN_div(secretKeyk1,rem, secretKeyk1, ten, ctx);
            BN_div(secretKeyk2,rem, secretKeyk2, ten, ctx);
        }
        BN_free(ten);
        BN_free(rem);
    }
    k1.secretKey = StringModifier::BN2LLI(secretKeyk1);
    k2.secretKey = StringModifier::BN2LLI(secretKeyk2);
    cout << ("6*7=%s\n", BN_bn2dec(secretKeyk1)) << endl;
    cout << ("6*7=%s\n", BN_bn2dec(secretKeyk2)) << endl;
    BN_CTX_free(ctx);
    return secretKeyk1;
}
