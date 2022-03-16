#include <stdio.h>
#include <math.h>
#include <ctime>

#include "SC.h"

    static long long int power(long long int a, long long int b,
            long long int P){
        if(b == 1)
            return a;
        return (((long long int) pow(a,b)) % P);
    };

    static bool isPrime(long long int number){
        if (number == 0 || number == 1) 
                return false;
                  
        for (int i = 2; i <= number/2; ++i) {
            if (number % i == 0) {
                return false;
            }
    
        }

        return true;    
    };

    static long long int getRandomPrime(){
        long long int rndNumb = (long long int)((rand() % 10000000) + 1);

        while(!isPrime(rndNumb))
            rndNumb++;

        return rndNumb;
    };

    static long long int generatePublicKey(){
        return getRandomPrime();
    }

