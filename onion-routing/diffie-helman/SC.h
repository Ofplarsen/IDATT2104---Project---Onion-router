#include <stdio.h>
#ifndef IDATT2104___PROJECT___ONION_ROUTER_SC_H
#define IDATT2104___PROJECT___ONION_ROUTER_SC_H


class SC{
    public:
        static long long int power(long long int a, long long int b,
                                                     long long int P);
        static long long int generatePublicKey();
        static long long int generatePublicKeyG();
    private:
        static bool isPrime(long long int number);
        static long long int getRandomPrime(int n);
        static int powerG(int x, unsigned int y, int p);
        static unordered_set<int> GeneratePrimes(int n);
        static int findPrimitiveRoot(int n);
        
};



 #endif //IDATT2104___PROJECT___ONION_ROUTER_SC_H
