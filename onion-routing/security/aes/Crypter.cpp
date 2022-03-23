//
// Created by xray2 on 16/03/2022.
//
#include <cmath>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <cstring>
#include <utility>
#include <openssl/err.h>
#include <iostream>
#include "../string-modifier/StringModifier.h"
#include "Crypter.h"
#include "../model/Cryption.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string.h>
#include <iostream>
#define BLOCK_SIZE 32

using namespace std;


int Crypter::decrypt(unsigned char* cipher, int cipher_len, unsigned char* key, unsigned char* text){
    int text_len = 0;
    int len = 0;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if(!ctx){
        perror(" EVP_CIPHER_CTX_new()");
        EVP_CIPHER_CTX_free(ctx);

        exit(-1);
    }

    if(!EVP_DecryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, NULL)){
        perror("EVP_EncryptInit_ex");
        exit(-1);
    }

    if(!EVP_DecryptUpdate(ctx, text, &len, cipher, cipher_len)){
        perror("EVP_EncryptUpdate(");
        EVP_CIPHER_CTX_free(ctx);

        exit(-1);
    }

    text_len += len;
    //EVP_CIPHER_CTX_set_padding(ctx, 0);
    if(!EVP_DecryptFinal_ex(ctx, text + len, &len)){
        BIO *bio = BIO_new(BIO_s_mem());
        ERR_print_errors(bio);
        char *buf;
        size_t len = BIO_get_mem_data(bio, &buf);
        string ret(buf, len);
        BIO_free(bio);
        EVP_CIPHER_CTX_free(ctx);
        cout << ret << endl;
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
        EVP_CIPHER_CTX_cleanup(ctx);
        exit(-1);
    }

    if(!EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, NULL)){
        perror("EVP_EncryptInit_ex");
        EVP_CIPHER_CTX_cleanup(ctx);
        exit(-1);
    }

    if(!EVP_EncryptUpdate(ctx, cipher, &len, text, text_len)){
        perror("EVP_EncryptUpdate(");
        EVP_CIPHER_CTX_cleanup(ctx);
        exit(-1);
    }

    cipher_len += len;

    if(!EVP_EncryptFinal_ex(ctx, cipher + len, &len)){
        perror("EVP_encrypt");
        EVP_CIPHER_CTX_cleanup(ctx);
        return -1;
    }

    cipher_len += len;
    EVP_CIPHER_CTX_cleanup(ctx);

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
        //std::string keyd = std::to_string(key);
        unsigned long t = strlen((const char*)(temp));

        int encrypted_len = encrypt(temp, t, StringModifier::convertToCharArray(std::to_string(key)), encrypted);

        char * copy = static_cast<char *>(malloc(encrypted_len));
        strcpy(copy, reinterpret_cast<const char *>(encrypted));

        c.strings_len.push_back(encrypted_len);
        c.res.push_back(reinterpret_cast<unsigned char *const>(copy));
    }

    return c;
}

Cryption Crypter::encryptString(Cryption &c, long long int key) {

    //std::vector<std::string> strings = StringModifier::splitString(text, 32);
    vector<unsigned char*> returnValue;
    std::vector<int> returnLengths;
    Cryption cr;
    for(int i = 0; i < c.strings_len.size(); i++){
        unsigned char* temp;
        temp = (unsigned char*)( c.res[i]);
        vector <unsigned char> test;

        unsigned char encrypted[64];
        memset(encrypted, 0, 64);
        std::string keyd = std::to_string(key);
        int encrypted_len = encrypt(temp, c.strings_len[i], StringModifier::convertToCharArray(std::to_string(key)), encrypted);

        char * copy = static_cast<char *>(malloc(encrypted_len));
        strcpy(copy, reinterpret_cast<const char *>(encrypted));

        cr.strings_len.push_back(encrypted_len);
        cr.res.push_back(reinterpret_cast<unsigned char *const>(copy));
    }

    return cr;
}

Cryption Crypter::encryptString(vector<string> strings, unsigned char* key) {

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
        int encrypted_len = encrypt(temp, strlen((const char*)(temp)), key, encrypted);

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

Cryption Crypter::decryptString(Cryption& cryption, unsigned char* key) {

    std::vector<unsigned char*> returnValue;
    std::vector<int> returnLengths;

    for(int i = 0; i < cryption.getRes().size(); i++){
        auto* temp = (unsigned char *) cryption.getRes()[i];
        unsigned char decrypted[cryption.strings_len[i]];
        memset(decrypted, 0, cryption.strings_len[i]);
        int dec_len = decrypt(temp, cryption.strings_len[i], key, decrypted);


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
    Cryption enc = Crypter::encryptString(stringToEncryptSplit, key);

    return enc;
}

Cryption Crypter::encrypt(Cryption &c, long long int key) {
    Cryption enc = Crypter::encryptString(c, key);
    return enc;
}

Cryption Crypter::decrypt(string stringToDecrypt,vector<int> lengths, long long int key) {
    vector<string> str = StringModifier::splitString(std::move(stringToDecrypt), BLOCK_SIZE);

    return decryptString(str, std::move(lengths),key);
}

Cryption Crypter::decrypt(Cryption &cryption, long long int key) {
    return decryptString(cryption, key);
}

Cryption Crypter::encrypt(string stringToEncrypt, unsigned char* key) {
    vector<string> stringToEncryptSplit = StringModifier::splitString(std::move(stringToEncrypt), BLOCK_SIZE);
    Cryption enc = Crypter::encryptString(stringToEncryptSplit, key);

    return enc;
}

Cryption Crypter::decrypt(Cryption &cryption, unsigned char* key) {
    return decryptString(cryption, key);
}

string Crypter::removePadding(string string1){

}