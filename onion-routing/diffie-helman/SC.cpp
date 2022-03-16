#include <stdio.h>
#include <math.h>
#include <ctime>
#include <bits/stdc++.h>
#include "SC.h"
#include <random>

using namespace std;

long long int SC::power(long long int a, long long int b,
        long long int P){
    if(b == 1)
        return a;
    return (((long long int) pow(a,b)) % P);
};

bool SC::isPrime(long long int number){
    if (number == 0 || number == 1)
            return false;

    for (int i = 2; i <= number/2; ++i) {
        if (number % i == 0) {
            return false;
        }

    }

    return true;
};

long long int SC::getRandomPrime(){
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(5, 1000000000);

    long long int rndNumb = distr(gen);

    while(!isPrime(rndNumb))
        rndNumb++;

    return rndNumb;
};

long long int SC::generatePublicKey(){
    return getRandomPrime();
};



long long int SC::generatePublicKeyG(int n){
    return 1;
};

int SC::powerG(int x, unsigned int y, int p){
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


unordered_set<int> SC::GeneratePrimes(int n){
    unordered_set<int> s;
    while(n%2 == 0){
        s.insert(2);
        n = n/2;
    }

    for(int i = 3; i <= sqrt(n);i =  i+2){
        while(n%i == 0){
            s.insert(i);
            n = n/i;
        }
    }

    if(n > 2)
        s.insert(n);
    return s;
}

int SC::findPrimitiveRoot(int n){
    unordered_set<int> s;
    if(!isPrime(n))
        return -1;

    int ETF = n - 1;
    s = GeneratePrimes(ETF);

    for(int r=2; r <= ETF; r++){
        bool flag = false;

        for(auto it = s.begin(); it != s.end(); it++){
            if(power(r, ETF/(*it), n) == 1){
                flag = true;
                break;
            }
        }
        if(!flag)
            return r;
    }
    return -1;
}
