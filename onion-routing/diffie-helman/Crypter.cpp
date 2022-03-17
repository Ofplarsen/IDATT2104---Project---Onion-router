//
// Created by xray2 on 16/03/2022.
//
#include <math.h>
#include "Crypter.h"

int Crypter::decrypt(int message, int key, int publicKey) {
    return modInverse(message*key, publicKey);
}

unsigned long long int Crypter::encrypt(int message, int key, int publicKey) {
    return (message * key)%publicKey;
}

int Crypter::modInverse(int a, int m)
{
    for (int x = 1; x < m; x++)
        if (((a%m) * (x%m)) % m == 1)
            return x;
}

