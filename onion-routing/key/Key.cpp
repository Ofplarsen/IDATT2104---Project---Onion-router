//
// Created by xray2 on 14/03/2022.
//

#include "Key.h"
#include "../diffie-helman/SC.h"

    long long int Key::generateKey(){
        generatedKey = SC::power(publicKeyG, privateKey ,publicKeyP);
        return generatedKey;
    };

    long long int Key::getSecretKey(long long int generatedKey){
        secretKey = SC::power(generatedKey, privateKey, publicKeyP);
        return secretKey;
    };

long long int Key::getPrivateKey() const {
    return privateKey;
}

void Key::setPrivateKey(long long int privateKey) {
    Key::privateKey = privateKey;
}

long long int Key::getPublicKeyP() const {
    return publicKeyP;
}

void Key::setPublicKeyP(long long int publicKeyP) {
    Key::publicKeyP = publicKeyP;
}

long long int Key::getPublicKeyG() const {
    return publicKeyG;
}

void Key::setPublicKeyG(long long int publicKeyG) {
    Key::publicKeyG = publicKeyG;
}

