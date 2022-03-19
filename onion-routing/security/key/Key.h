//
// Created by xray2 on 14/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_KEY_H
#define IDATT2104___PROJECT___ONION_ROUTER_KEY_H

/**
 * Class that hold public, private, gen, and secret keys for node
 */
class Key {
private:
    unsigned long long int privateKey;
    unsigned long long int publicKeyP;
    unsigned long long int publicKeyG;
    unsigned long long int generatedKey;
    unsigned long long int secretKey;


public:
    Key();

    unsigned long long int generateKey();
    unsigned long long int getSecretKey(long long int generatedKey);

    unsigned long long int getPrivateKey() const;

    void setPrivateKey(long long int privateKey);

    unsigned long long int getPublicKeyP() const;

    void setPublicKeyP(long long int publicKeyP);

    unsigned long long int getPublicKeyG() const;

    void setPublicKeyG(long long int publicKeyG);


};


#endif //IDATT2104___PROJECT___ONION_ROUTER_KEY_H
