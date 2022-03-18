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
#include "onion-routing/security/model/Cryption.h"
#include "onion-routing/security/string-modifier/StringModifier.h"
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

unsigned char* convertToCharArray(long long int a)
{
    unsigned char* arr;
    int i = 0;

    for (i = 0; i < 8; ++i)
    {
        arr[i] = (unsigned char)((((unsigned long long) a) >> (56 - (8*i))) & 0xFFu);
    }
    return arr;
}



int main(int argc, char const *argv[]){
    SC::initRandom();
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

    annaKey.setPrivateKey(2354);
    bobKey.setPrivateKey(13132);
    unsigned long long int secretKey = annaKey.getSecretKey(bobKey.generateKey());
    cout << secretKey << endl;
    secretKey = secretKey / 10000;
    cout << secretKey << endl;
    string textTest = "This is a longer messa";
    //unsigned char* key = convertToCharArray(secretKey);
    unsigned char* key = (unsigned char*)"1234567890123456";

    unsigned char* key2 = (unsigned char*)"1234567890123457";
    std::string text = "My secret message is now longerr hello ";
    vector<char> textAsChar = StringModifier::stringToCharVector(text);

    vector<vector<char>> cha = StringModifier::stringToCharArray(text);



    vector<int> t;

    Cryption cryption(cha);

    //int text_len = strlen((const char*)text);
    //unsigned char cipher[64];

    //printf("cipher=\n");

    //int cipher_len = Crypter::encrypt(text, text_len, key, cipher);
    Crypter c;
    vector<unsigned char*> enc = c.encryptString(cryption, 1234567890123456);

    for(int i = 0; i < enc.size(); i++){
        for(int y = 0; y < 32; y++){
            printf("%02x ", enc[i][y]);
        }
    }

    /*
    printf("decrypted = \n");
    unsigned char decrypted[64];
    int dec_len = Crypter::decrypt(cipher2, cipher2_len, key2,decrypted);
    for(int i = 0; i < dec_len; i++){
        printf("%c", (const char)decrypted[i]);
    }

     */

    return 0;
}