//
// Created by xray2 on 14/03/2022.
//
#include "Key.h"
#include "../diffie-helman/SC.h"
#define P 1786534726771898
#define G 1234567890123

/**
 * Generates a key by using publicG, private and P key
 * @return
 */
unsigned long long int Key::generateKey(){
    generatedKey = SC::power(publicKeyG, privateKey ,publicKeyP);
    return generatedKey;
};

/**
 * Generates a secret key of generatedKey, private key and public key
 * @param generatedKey
 * @return
 */
unsigned long long int Key::getSecretKey(long long int generatedKey){
        secretKey = SC::power(generatedKey, privateKey, publicKeyP);
        return secretKey;
    };

unsigned long long int Key::getPrivateKey() const {
    return privateKey;
}

void Key::setPrivateKey(long long int privateKey) {
    Key::privateKey = privateKey;
}

unsigned long long int Key::getPublicKeyP() const {
    return publicKeyP;
}

void Key::setPublicKeyP(long long int publicKeyP) {
    Key::publicKeyP = publicKeyP;
}

unsigned long long int Key::getPublicKeyG() const {
    return publicKeyG;
}

void Key::setPublicKeyG(long long int publicKeyG) {
    Key::publicKeyG = publicKeyG;
}
/**
 * Constructor that inits the keys with standard values, and a random prime private key
 */
Key::Key() {
    privateKey = SC::generatePrivateKey();
    publicKeyP = P;
    publicKeyG = G;
}

