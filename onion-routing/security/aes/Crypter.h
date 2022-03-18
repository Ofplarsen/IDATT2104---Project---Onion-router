//
// Created by xray2 on 16/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_CRYPTER_H
#define IDATT2104___PROJECT___ONION_ROUTER_CRYPTER_H


class Crypter {
public:
    static int decrypt(unsigned char* cipher, int cipher_len, unsigned char* key, unsigned char* text);
    static int encrypt(unsigned char* text, int text_len, unsigned char* key, unsigned char* cipher);
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_CRYPTER_H
