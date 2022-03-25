//
// Created by xray2 on 14/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_KEY_H
#define IDATT2104___PROJECT___ONION_ROUTER_KEY_H

#include <stdio.h>
#include <openssl/dh.h>
#include <openssl/bn.h>

/**
 * Class that hold public, private, gen, and secret keys for node
 */
class Key {
private:
    BIGNUM *privateKey;
    BIGNUM *publicKeyP;
    BIGNUM *publicKeyG;
    BIGNUM *generatedKey;

public:
    long long secretKey;
    Key();

    BIGNUM * generateKey();
    //unsigned long long int getSecretKey(long long int generatedKey);


    BIGNUM * getSecretKey(BIGNUM *generatedKey);

    BIGNUM *getSecretKey1() const;
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_KEY_H
