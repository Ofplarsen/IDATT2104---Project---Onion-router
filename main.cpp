#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>
#include "onion-routing/node/Node.h"
#include "onion-routing/node/ExitNode.h"
#include "onion-routing/diffie-helman/SC.h"
#include "onion-routing/diffie-helman/Crypter.h"
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

#define PORT 8080




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


    string s = to_string(secretKey);
    unsigned char key[10+ sizeof (char)];
    unsigned char text[]="hello world this is another test";
    unsigned char enc_out[80];
    unsigned char dec_out[80];
    std::sprintf((char *) key, "%d", secretKey);

    AES_KEY enc_key, dec_key;

    AES_set_encrypt_key(key, 128, &enc_key);
    AES_encrypt(text, enc_out, &enc_key);

    AES_set_decrypt_key(key,128,&dec_key);
    AES_decrypt(enc_out, dec_out, &dec_key);

    int i;

    printf("original:\t");
    for(i=0;*(text+i)!=0x00;i++)
        printf("%X ",*(text+i));
    printf("\nencrypted:\t");
    for(i=0;*(enc_out+i)!=0x00;i++)
        printf("%X ",*(enc_out+i));
    printf("\ndecrypted:\t");
    for(i=0;*(dec_out+i)!=0x00;i++)
        printf("%X ",*(dec_out+i));
    printf("\n");


    return 0;
}
