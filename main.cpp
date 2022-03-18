#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>
#include "onion-routing/node/Node.h"
#include "onion-routing/node/ExitNode.h"
#include "onion-routing/security/diffie-helman/SC.h"
#include "onion-routing/security/aes/Crypter.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <openssl/aes.h>
#include <cmath>
#include <vector>
#define PORT 8080

int main(int argc, char const *argv[]){

    Key annaKey;
    Key bobKey;
    long long int publicKey = SC::generatePublicKey();
    long long int primitiveRoot = SC::findPrimitiveRoot(publicKey);
    annaKey.setPublicKeyP(publicKey);
    annaKey.setPublicKeyG(primitiveRoot);
    bobKey.setPublicKeyP(publicKey);
    bobKey.setPublicKeyG(primitiveRoot);

    cout << publicKey << endl;
    cout << primitiveRoot << endl;

    annaKey.setPrivateKey(103);
    bobKey.setPrivateKey(101);
    unsigned long long int secretKey = annaKey.getSecretKey(bobKey.generateKey());
    secretKey = secretKey / 10000;
    cout << secretKey << endl;

    unsigned char* key = (unsigned char*) "1234567890123456";
    unsigned char* text = (unsigned char*)"My secret message is now longer";

    int text_len = strlen((const char*)text);
    unsigned char cipher[64];

    printf("cipher=\n");

    int cipher_len = Crypter::encrypt(text, text_len, key, cipher);

    for(int i = 0; i < cipher_len; i++){
        printf("%02x ", cipher[i]);
    }
    printf("\n");

    printf("decrypted = \n");
    unsigned char decrypted[64];
    int dec_len = Crypter::decrypt(cipher, cipher_len, key,decrypted);

    for(int i = 0; i < dec_len; i++){
        printf("%c", (const char)decrypted[i]);
    }
    printf("\n");

    return 0;
}

/*
int main(int argc, char const *argv[])
{

    Key annaKey;
    Key bobKey;
    long long int publicKey = SC::generatePublicKey();
    long long int primitiveRoot = SC::findPrimitiveRoot(publicKey);
    annaKey.setPublicKeyP(publicKey);
    annaKey.setPublicKeyG(primitiveRoot);
    bobKey.setPublicKeyP(publicKey);
    bobKey.setPublicKeyG(primitiveRoot);

    cout << publicKey << endl;
    cout << primitiveRoot << endl;

    annaKey.setPrivateKey(103);
    bobKey.setPrivateKey(101);
    unsigned long long int secretKey = annaKey.getSecretKey(bobKey.generateKey());
    secretKey = secretKey / 10000;
    cout << secretKey << endl;
    //cout << bobKey.getSecretKey(annaKey.generateKey()) << endl;
    //int encrypted = Crypter::encrypt('d', annaKey.getSecretKey(bobKey.generateKey()), annaKey.getPublicKeyP());

    //cout << annaKey.getSecretKey(bobKey.generateKey()) << endl;
    //cout << annaKey.getPublicKeyP() << endl;
    //cout << (Crypter::decrypt(encrypted, annaKey.getSecretKey(bobKey.generateKey()), annaKey.getPublicKeyP())) << endl;

}
*/