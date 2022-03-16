#include <stdio.h>
#include <unordered_set>

#ifndef IDATT2104___PROJECT___ONION_ROUTER_SC_H
#define IDATT2104___PROJECT___ONION_ROUTER_SC_H


class SC{
    public:

        static long long int power(long long int a, long long int b,
                                                     long long int P);
        static long long int generatePublicKey();
        static long long int generatePublicKeyG(int n);
    //private:
        static bool isPrime(long long int number);
        static long long int getRandomPrime();
        static int powerG(int x, unsigned int y, int p);
        static std::unordered_set<int> GeneratePrimes(int n);
        static int findPrimitiveRoot(int n);

};



 #endif //IDATT2104___PROJECT___ONION_ROUTER_SC_H
