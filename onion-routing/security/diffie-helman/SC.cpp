
#include <cmath>
#include <ctime>
#include <bits/stdc++.h>
#include "SC.h"

#include <stdio.h>
#include <openssl/dh.h>
#include <openssl/bn.h>


using namespace std;
/**
 * Method for finding pow(a,b) and finding mod P
 * @param a
 * @param b
 * @param p
 * @return
 */
unsigned long long int SC::power(long long int a, long long int b,
        long long int p){
    if(b == 1)
        return a;
    return (((unsigned long long int) pow(a,b)) % p);
};

BIGNUM * SC::power(BIGNUM *a, BIGNUM *b, BIGNUM *p){
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *returnValue = BN_new();
    BN_mod_exp(returnValue, a, b, p, ctx);
    BN_CTX_free(ctx);
    return returnValue;
};

/**
 * Method that checks if a number is prime
 * @param number
 * @return
 */

/**
 * Method that generates random number and adds 1 until it is prime
 * TODO optimise?
 * @return
 */
BIGNUM * SC::getRandomPrime(){
    BIGNUM *ret = BN_new();
    BN_generate_prime_ex(ret, 224,1 ,nullptr, nullptr, nullptr);
    return ret;
};

/**
 * Method that has a cleaner name than randomPrime
 * @return
 */
BIGNUM * SC::generatePrivateKey(){
    return getRandomPrime();
};

//TODO REMOVE THESE IF no use
unsigned long long int SC::powerG(int x, unsigned int y, int p){
    int res = 1;

    x = x % p;
    while(y > 0){
        if(y & 1)
            res = (res * x) % p;
        y = y >> 1;
        x = (x*x) % p;
    }
    return res;
}

