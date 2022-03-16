#include <stdio.h>
#include <math.h>
#include <ctime>
#include <bits/stdc++.h>
#include "SC.h"

using namespace std;

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
    };



static long long int generatePublicKeyG(int n){
    return (long long int) findPrimitiveRoot(n);
};

static int powerG(int x, unsigned int y, int p){
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


static unordered_set<int> GeneratePrimes(int n){
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

static int findPrimitiveRoot(int n){
    unordered_set<int> s;
    if(isPrime(n) == false)
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


