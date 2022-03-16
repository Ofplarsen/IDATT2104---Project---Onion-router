
#ifndef IDATT2104___PROJECT___ONION_ROUTER_SC_H
#define IDATT2104___PROJECT___ONION_ROUTER_SC_H

class SC{
    public:
        static long long int power(long long int a, long long int b,
                                                     long long int P);
        static long long int generatePublicKey();

    private:
        static bool isPrime(long long int number);
        static long long int getRandomPrime();
};



 #endif //IDATT2104___PROJECT___ONION_ROUTER_SC_H
