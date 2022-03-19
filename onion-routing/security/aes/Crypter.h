//
// Created by xray2 on 16/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_CRYPTER_H
#define IDATT2104___PROJECT___ONION_ROUTER_CRYPTER_H


#include <string>
#include "../model/Cryption.h"

class Crypter {
public:
    static int decrypt(unsigned char* cipher, int cipher_len, unsigned char* key, unsigned char* text);
    static int encrypt(unsigned char* text, int text_len, unsigned char* key, unsigned char* cipher);
    std::vector<unsigned char*> encryptString(string text, long long int key);
    //static Cryption decryptString(Cryption cryption, long long int key);

    static Cryption encryptString(vector<string> strings, long long int key);

    static Cryption decryptString(Cryption &cryption, long long int key);

    static void encryptString(vector<string> strings, long long int key, Cryption & c);

    //Cryption encryptString(vector<string> strings, long long int key, Cryption &c);
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_CRYPTER_H
