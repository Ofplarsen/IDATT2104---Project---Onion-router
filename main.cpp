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


#define P 1786534726771898
#define G 1234567890123
int main(int argc, char const *argv[]){
    SC::initRandom();
    Key annaKey;
    Key bobKey;
    long long int publicKey = P;
    long long int primitiveRoot = G;
    annaKey.setPublicKeyP(publicKey);
    annaKey.setPublicKeyG(primitiveRoot);
    bobKey.setPublicKeyP(publicKey);
    bobKey.setPublicKeyG(primitiveRoot);

    cout << publicKey << endl;
    cout << primitiveRoot << endl;

    annaKey.setPrivateKey(1786534726771898);
    bobKey.setPrivateKey(1786534726771898);
    unsigned long long int secretKey1 = annaKey.getSecretKey(bobKey.generateKey());
    long long int secretKey = secretKey1 / 10000;
    cout << secretKey1 << endl;
    cout << secretKey << endl;
    //std::string text = "hello please work. I hate debugging :)))";
    std::string text = "GET / HTTP/1.1\r\n"
                       "Host: localhost:1250\r\n"
                       "Connection: keep-alive\r\n"
                       "Cache-Control: max-age=0\r\n"
                       "sec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"99\", \"Google Chrome\";v=\"99\"\r\n"
                       "sec-ch-ua-mobile: ?0\r\n"
                       "sec-ch-ua-platform: \"Windows\"\r\n"
                       "Upgrade-Insecure-Requests: 1\r\n";

    Cryption enc = Crypter::encrypt(text, 1234567890123456);
    string encrypted;
    for(int i = 0; i < enc.getRes().size(); i++){

        for(int y = 0; y < enc.strings_len[i]; y++){
            encrypted += enc.getRes()[i][y];
        }
    }
    cout << encrypted << endl;

    Cryption dec = Crypter::decrypt(enc, 1234567890123456);
    string decrypted;
    for(int i = 0; i < dec.getRes().size(); i++){
        for(int y = 0; y < dec.strings_len[i]; y++){
            decrypted += dec.getRes()[i][y];
        }
    }
    cout << decrypted << endl;

//    Cryption cryption;
//    cryption.setTest(text);
//
//    Cryption enc = Crypter::encryptString(text1, 1234567890123456);
//    vector<string> str;
//    for(int i = 0; i < enc.getRes().size(); i++){
//        string encrypted;
//        for(int y = 0; y < enc.strings_len[i]; y++){
//            encrypted += enc.getRes()[i][y];
//        }
//        str.push_back(encrypted);
//    }

    //Node n;

    //cout << encrypted << endl;

    //vector<string> str = StringModifier::splitString(encrypted, 32);

//    Cryption dec = Crypter::decryptString(str, enc.strings_len,1234567890123456);
//    string decrypted;
//    for(int i = 0; i < dec.getRes().size(); i++){
//        for(int y = 0; y < dec.strings_len[i]; y++){
//            decrypted += dec.getRes()[i][y];
//        }
//    }
//    cout << decrypted << endl;
//    return 0;
}