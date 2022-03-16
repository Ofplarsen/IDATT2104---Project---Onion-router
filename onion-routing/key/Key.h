//
// Created by xray2 on 14/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_KEY_H
#define IDATT2104___PROJECT___ONION_ROUTER_KEY_H


class Key {
private:
    long long int privateKey;
    long long int publicKeyP;
    long long int publicKeyG;
    long long int generatedKey;
    long long int secretKey;


public:
    long long int generateKey();
    long long int getSecretKey(long long int generatedKey);

    long long int getPrivateKey() const;

    void setPrivateKey(long long int privateKey);

    long long int getPublicKeyP() const;

    void setPublicKeyP(long long int publicKeyP);

    long long int getPublicKeyG() const;

    void setPublicKeyG(long long int publicKeyG);


};


#endif //IDATT2104___PROJECT___ONION_ROUTER_KEY_H
