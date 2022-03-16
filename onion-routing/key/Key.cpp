//
// Created by xray2 on 14/03/2022.
//

#include "Key.h"
#include "../SC/SC.h"

class Key {

    long long int generateKey(){
        generatedKey = SC.power(publicKey2, privateKey ,publicKey1);
        return generatedKey;
    }

    long long int getSecretKey(){
        secretKey = SC.power(generatedKey, privateKey, publicKey);
        return secretKey;
    }
}
