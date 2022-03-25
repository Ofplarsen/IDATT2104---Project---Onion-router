#include <stdio.h>
#include <unordered_set>

#include <stdio.h>
#include <openssl/dh.h>
#include <openssl/bn.h>

#ifndef IDATT2104___PROJECT___ONION_ROUTER_SC_H
#define IDATT2104___PROJECT___ONION_ROUTER_SC_H

/**
 * Class for generating parts of diffie Hellman handshake
 */
class SC{
    public:


    static BIGNUM * generatePrivateKey();

    static bignum_st *power(bignum_st *a, bignum_st *b, bignum_st *p);
private:
        static BIGNUM * getRandomPrime();


};



 #endif //IDATT2104___PROJECT___ONION_ROUTER_SC_H
