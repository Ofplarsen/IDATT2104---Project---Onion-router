//
// Created by xray2 on 14/03/2022.
//

#include "Key.h"
#include "../diffie-helman/SC.h"

    long long int Key::generateKey(){
        generatedKey = SC.power(publicKey2, privateKey ,publicKey1);
        return generatedKey;
    };

    long long int Key::getSecretKey(){
        secretKey = SC.power(generatedKey, privateKey, publicKey);
        return secretKey;
    };
