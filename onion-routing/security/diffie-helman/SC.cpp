
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
    BN_generate_prime_ex(ret, 144,1 ,NULL, NULL, NULL);
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

//
//unordered_set<int> SC::GeneratePrimes(int n){
//    unordered_set<int> s;
//    while(n%2 == 0){
//        s.insert(2);
//        n = n/2;
//    }
//
//    for(int i = 3; i <= sqrt(n);i =  i+2){
//        while(n%i == 0){
//            s.insert(i);
//            n = n/i;
//        }
//    }
//
//    if(n > 2)
//        s.insert(n);
//    return s;
//}

//bool SC::isPrime2(long long int n)
//{
//    // Corner cases
//    if (n <= 1)  return false;
//    if (n <= 3)  return true;
//
//    // This is checked so that we can skip
//    // middle five numbers in below loop
//    if (n%2 == 0 || n%3 == 0) return false;
//
//    for (int i=5; i*i<=n; i=i+6)
//        if (n%i == 0 || n%(i+2) == 0)
//            return false;
//
//    return true;
//}

//int SC::findPrimitiveRoot(int n){
//    unordered_set<int> s;
//    if(!isPrime2(n))
//        return -1;
//
//    int ETF = n - 1;
//    s = GeneratePrimes(ETF);
//
//    for(int r=2; r <= ETF; r++){
//        bool flag = false;
//
//        for(auto it = s.begin(); it != s.end(); it++){
//            if(powerG(r, ETF/(*it), n) == 1){
//                flag = true;
//                break;
//            }
//        }
//        if(!flag)
//            return r;
//    }
//    return -1;
//}
