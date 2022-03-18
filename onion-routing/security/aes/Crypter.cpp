//
// Created by xray2 on 16/03/2022.
//
#include <cmath>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <cstring>
#include "../string-modifier/StringModifier.h"
#include "Crypter.h"
#include "../model/Cryption.h"
int Crypter::decrypt(unsigned char* cipher, int cipher_len, unsigned char* key, unsigned char* text){
    int text_len = 0;
    int len = 0;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if(!ctx){
        perror(" EVP_CIPHER_CTX_new()");
        exit(-1);
    }

    if(!EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL)){
        perror("EVP_EncryptInit_ex");
        exit(-1);
    }

    if(!EVP_DecryptUpdate(ctx, text, &len, cipher, cipher_len)){
        perror("EVP_EncryptUpdate(");
        exit(-1);
    }

    text_len += len;

    if(!EVP_DecryptFinal_ex(ctx, text + len, &len)){
        perror("EVP_DencryptFinal_ex");
        exit(-1);
    }

    text_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return text_len;
}

int Crypter::encrypt(unsigned char* text, int text_len, unsigned char* key, unsigned char* cipher){
    int cipher_len = 0;
    int len = 0;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if(!ctx){
        perror(" EVP_CIPHER_CTX_new()");
        exit(-1);
    }

    if(!EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL)){
        perror("EVP_EncryptInit_ex");
        exit(-1);
    }

    if(!EVP_EncryptUpdate(ctx, cipher, &len, text, text_len)){
        perror("EVP_EncryptUpdate(");
        exit(-1);
    }

    cipher_len += len;

    if(!EVP_EncryptFinal_ex(ctx, cipher + len, &len)){
        perror("EVP_EncryptFinal_ex");
        exit(-1);
    }

    cipher_len += len;
    EVP_CIPHER_CTX_free(ctx);

    return cipher_len;
}

vector<unsigned char*> Crypter::encryptString(Cryption cryption,long long int key) {

    //std::vector<std::string> strings = StringModifier::splitString(text, 32);

    vector<unsigned char*> returnValue;
    std::vector<int> returnLengths;
    for(auto & string : cryption.getStrings()){
        auto* temp = (unsigned char *) &string[0];
        unsigned char encrypted[64];
        std::string keyd = std::to_string(key);
        int encrypted_len = encrypt(temp, strlen((const char*)(temp)), StringModifier::convertToCharArray(std::to_string(key)), encrypted);

        returnLengths.emplace_back(encrypted_len);
        returnValue.emplace_back(encrypted);
    }

    for(int i = 0; i < returnValue.size(); i++){
        for(int y = 0; y < returnLengths[i]; y++){
            printf("%02x ", returnValue[i][y]);
        }
    }

    decryptString(Cryption(returnValue, returnLengths), key);

    return returnValue;//Cryption(returnValue, returnLengths);
}

Cryption Crypter::decryptString(Cryption cryption, long long int key) {

    std::vector<unsigned char*> returnValue;
    std::vector<int> returnLengths;

    for(auto & string : cryption.getRes()){
        auto* temp = (unsigned char *) &string[0];
        unsigned char decrypted[64];
        int dec_len = decrypt(temp, strlen((const char*)(temp)), StringModifier::convertToCharArray(std::to_string(key)), decrypted);

        returnLengths.emplace_back(dec_len);
        returnValue.emplace_back((decrypted));
    }

    for(int i = 0; i < returnValue.size(); i++){
        for(int y = 0; y < returnLengths[i]; y++){
            printf("%c ", (const char)returnValue[i][y]);
        }
    }


    return Cryption();
}

