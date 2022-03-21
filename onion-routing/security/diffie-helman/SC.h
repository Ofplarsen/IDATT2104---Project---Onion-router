#include <stdio.h>
#include <unordered_set>
#include "../big-int/BigInt.h"

#ifndef IDATT2104___PROJECT___ONION_ROUTER_SC_H
#define IDATT2104___PROJECT___ONION_ROUTER_SC_H

/**
 * Class for generating parts of diffie Hellman handshake
 */
class SC{
    public:

        static unsigned long long int power(long long int a, long long int b,
                                                     long long int P);
        static unsigned long long int generatePrivateKey();

    static unsigned long long int powerG(int x, unsigned int y, int p);

private:
        static bool isPrime(long long int number);
        static unsigned long long int getRandomPrime();

    static std::unordered_set<int> GeneratePrimes(int n);
        static int findPrimitiveRoot(int n);

        static bool isPrime2(long long int n);
        static void initRandom();

    BigInt power(BigInt &a, BigInt &b, BigInt &p);
};



 #endif //IDATT2104___PROJECT___ONION_ROUTER_SC_H
