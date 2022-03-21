#include <iostream>
#include "onion-routing/node/Node.h"
#include "onion-routing/security/diffie-helman/SC.h"
#include "onion-routing/security/aes/Crypter.h"
#include "onion-routing/security/model/Cryption.h"
#include "onion-routing/security/string-modifier/StringModifier.h"
#include "onion-routing/client/MainServer.h"
#include <vector>
#include <ctime>


#define P 1786534726771898
#define G 1234567890123
int main(int argc, char const *argv[]){

    MainServer main;

    vector<Node> nodes = main.getUserNodes();

    std::string text = "GET / HTTP/1.1\r\n"
                       "Host: localhost:1250\r\n"
                       "Connection: keep-alive\r\n"
                       "Cache-Control: max-age=0\r\n"
                       "sec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"99\", \"Google Chrome\";v=\"99\"\r\n"
                       "sec-ch-uadsadsadsa";

    //Cryption enc = Crypter::encrypt(text, BN_get_word(nodes[0].decryptKey.getSecretKey1()));

    Cryption enc = Crypter::encrypt(text,StringModifier::BN2LLI(nodes[0].decryptKey.secretKey));
    string encrypted;
    for(int i = 0; i < enc.getRes().size(); i++){
        for(int y = 0; y < enc.strings_len[i]; y++){
            encrypted += enc.getRes()[i][y];
        }
    }
    // Cryption dec = Crypter::decrypt(enc, BN_get_word(nodes[1].encryptKey.getSecretKey1()));
    Cryption dec = Crypter::decrypt(enc, StringModifier::BN2LLI(nodes[1].encryptKey.secretKey));
    string decrypted;
    for(int i = 0; i < dec.getRes().size(); i++){
        for(int y = 0; y < dec.strings_len[i]; y++){
            decrypted += dec.getRes()[i][y];
        }
    }
    cout << decrypted << endl;

//    Key annaKey;
//    Key bobKey;
//    long long int publicKey = P;
//    long long int primitiveRoot = G;
//    annaKey.setPublicKeyP(publicKey);
//    annaKey.setPublicKeyG(primitiveRoot);
//    bobKey.setPublicKeyP(publicKey);
//    bobKey.setPublicKeyG(primitiveRoot);
//
//    cout << annaKey.getPrivateKey() << endl;
//    cout << bobKey.getPrivateKey() << endl;
//
//    unsigned long long int secretKey1 = annaKey.getSecretKey(bobKey.generateKey());
//    long long int secretKey = secretKey1 / 10000;
//    cout << secretKey1 << endl;
//    cout << secretKey << endl;
//    //std::string text = "hello please work. I hate debugging :)))";
//    std::string text = "GET / HTTP/1.1\r\n"
//                       "Host: localhost:1250\r\n"
//                       "Connection: keep-alive\r\n"
//                       "Cache-Control: max-age=0\r\n"
//                       "sec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"99\", \"Google Chrome\";v=\"99\"\r\n"
//                       "sec-ch-ua-mobile: ?0\r\n"
//                       "sec-ch-ua-platform: \"Windows\"\r\n"
//                       "Upgrade-Insecure-Requests: 1\r\n\r\n";
//
//    Cryption enc = Crypter::encrypt(text, 1234567890123456);
//    string encrypted;
//    for(int i = 0; i < enc.getRes().size(); i++){
//
//        for(int y = 0; y < enc.strings_len[i]; y++){
//            encrypted += enc.getRes()[i][y];
//        }
//    }
//    cout << encrypted << endl;
//
//    Cryption dec = Crypter::decrypt(enc, 1234567890123456);
//    string decrypted;
//    for(int i = 0; i < dec.getRes().size(); i++){
//        for(int y = 0; y < dec.strings_len[i]; y++){
//            decrypted += dec.getRes()[i][y];
//        }
//    }
//    cout << decrypted << endl;
    return 0;
}