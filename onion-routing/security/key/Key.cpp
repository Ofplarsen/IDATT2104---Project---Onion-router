//
// Created by xray2 on 14/03/2022.
//
#include "Key.h"
#include "../diffie-helman/SC.h"
#include "../string-modifier/StringModifier.h"
#define G 134547571
#define P 217436006305459749
/**
 * Generates a key by using publicG, private and P key
 * @return
 */
BIGNUM * Key::generateKey(){
    generatedKey = SC::power(publicKeyG, privateKey ,publicKeyP);
    return generatedKey;
};

/**
 * Generates a secret key of generatedKey, private key and public key
 * @param generatedKey
 * @return
 */
BIGNUM * Key::getSecretKey(BIGNUM *generatedKey){
        return SC::power(generatedKey, privateKey, publicKeyP);
    };


/**
 * Constructor that inits the keys with standard values, and a random prime private key
 */
Key::Key() {
    privateKey = SC::generatePrivateKey();
    publicKeyP = BN_new();
    BN_set_word(publicKeyP, P);
    publicKeyG = BN_new();
    BN_set_word(publicKeyG, G);
}




