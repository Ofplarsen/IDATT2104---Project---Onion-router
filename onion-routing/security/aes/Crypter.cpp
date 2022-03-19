//
// Created by xray2 on 16/03/2022.
//
#include <cmath>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <cstring>
#include <utility>
#include "../string-modifier/StringModifier.h"
#include "Crypter.h"
#include "../model/Cryption.h"

#define BLOCK_SIZE 32
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

Cryption Crypter::encryptString(vector<string> strings, long long int key) {

    //std::vector<std::string> strings = StringModifier::splitString(text, 32);
    vector<unsigned char*> returnValue;
    std::vector<int> returnLengths;
    Cryption c;
    for(int i = 0; i < strings.size(); i++){
        unsigned char* temp;
        temp = (unsigned char*)( strings[i].c_str());
        vector <unsigned char> test;

        unsigned char encrypted[64];
        memset(encrypted, 0, 64);
        std::string keyd = std::to_string(key);
        int encrypted_len = encrypt(temp, strlen((const char*)(temp)), StringModifier::convertToCharArray(std::to_string(key)), encrypted);

        char * copy = static_cast<char *>(malloc(encrypted_len));
        strcpy(copy, reinterpret_cast<const char *>(encrypted));

        c.strings_len.push_back(encrypted_len);
        c.res.push_back(reinterpret_cast<unsigned char *const>(copy));
    }

//    for(int i = 0; i < returnValue.size(); i++){
//        for(int y = 0; y < returnLengths[i]; y++){
//            printf("%02x ", returnValue[i][y]);
//        }
//    }

    return c;
}

Cryption Crypter::decryptString(Cryption& cryption, long long int key) {

    std::vector<unsigned char*> returnValue;
    std::vector<int> returnLengths;

    for(int i = 0; i < cryption.getRes().size(); i++){
        auto* temp = (unsigned char *) cryption.getRes()[i];
        unsigned char decrypted[64];
        memset(decrypted, 0, 64);
        int dec_len = decrypt(temp, cryption.strings_len[i], StringModifier::convertToCharArray(std::to_string(key)), decrypted);


        char * copy = static_cast<char *>(malloc(cryption.getStringsLen()[i]));
        strcpy(copy, reinterpret_cast<const char *>(decrypted));

        returnLengths.emplace_back(dec_len);
        returnValue.emplace_back(reinterpret_cast<unsigned char *const>(copy));
    }

    Cryption c;
    c.setRes(returnValue);
    c.setStringsLen(returnLengths);

    return c;
}

Cryption Crypter::decryptString(vector<string> strings,vector<int> lengths, long long int key) {

    std::vector<unsigned char*> returnValue;
    std::vector<int> returnLengths;

    for(int i = 0; i < strings.size(); i++){
        auto* temp = (unsigned char *) strings[i].c_str();
        unsigned char decrypted[64];
        memset(decrypted, 0, 64);
        int dec_len = decrypt(temp, lengths[i], StringModifier::convertToCharArray(std::to_string(key)), decrypted);


        char * copy = static_cast<char *>(malloc(dec_len));
        strcpy(copy, reinterpret_cast<const char *>(decrypted));

        returnLengths.emplace_back(dec_len);
        returnValue.emplace_back(reinterpret_cast<unsigned char *const>(copy));
    }

    Cryption c;
    c.setRes(returnValue);
    c.setStringsLen(returnLengths);

    return c;
}

Cryption Crypter::encrypt(string stringToEncrypt, long long int key) {
    vector<string> stringToEncryptSplit = StringModifier::splitString(std::move(stringToEncrypt), BLOCK_SIZE);
    Cryption enc = Crypter::encryptString(stringToEncryptSplit, 1234567890123456);

    return enc;
}

Cryption Crypter::decrypt(string stringToDecrypt,vector<int> lengths, long long int key) {
    vector<string> str = StringModifier::splitString(std::move(stringToDecrypt), BLOCK_SIZE);

    return decryptString(str, std::move(lengths),key);
}

Cryption Crypter::decrypt(Cryption &cryption, long long int key) {
    return decryptString(cryption, key);
}

