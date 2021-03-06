//
// Created by xray2 on 16/03/2022.
//
#include <cmath>
#include <openssl/evp.h>
#include <utility>
#include <openssl/err.h>
#include <iostream>
#include "../string-modifier/StringModifier.h"
#include "Crypter.h"
#include <string.h>

#define BLOCK_SIZE 32

using namespace std;

/**
 * Method that decrypts a bock of chars with given key
 */
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

/**
 * Method that encrypts a block of chars with given key
 * @param text
 * @param text_len
 * @param key
 * @param cipher
 * @return
 */
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

/**
* Method that encrypts a vector of strings with given key in a char array
* @param strings
* @param key
* @return
*/
Cryption Crypter::encryptString(vector<string> strings, long long int key) {

    vector<unsigned char*> returnValue;
    std::vector<int> returnLengths;
    Cryption c;
    for(int i = 0; i < strings.size(); i++){
        unsigned char* temp;
        temp = (unsigned char*)( strings[i].c_str());
        vector <unsigned char> test;

        unsigned char encrypted[64];
        memset(encrypted, 0, 64);
        unsigned long t = strlen((const char*)(temp));

        int encrypted_len = encrypt(temp, t, StringModifier::convertToCharArray(std::to_string(key)), encrypted);

        char * copy = static_cast<char *>(malloc(encrypted_len));
        strcpy(copy, reinterpret_cast<char *>(encrypted));

        c.strings_len.push_back(encrypted_len);
        c.res.push_back(reinterpret_cast<unsigned char *>(copy));
    }

    return c;
}

/**
 * Method that encrypts a Cryption with given key
 * @param c
 * @param key
 * @return
 */
Cryption Crypter::encryptString(Cryption &c, long long int key) {

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

/**
 * Method that encrypts a vector of strings with given key in a char array
 * @param strings
 * @param key
 * @return
 */
Cryption Crypter::encryptString(vector<string> strings, unsigned char* key) {

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

    return c;
}

/**
 * Method that decrypts a Cryption with given key (charArray)
 * @param cryption
 * @param key
 * @return
 */
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

/**
 * Method that decrypts a Cryption with given key (long long int)
 * @param cryption
 * @param key
 * @return
 */
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

/**
 * Method that decrypts a vector of strings with block sizes (and offset) with given key
 * @param strings
 * @param lengths
 * @param key
 * @return
 */
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

/**
 * Method that encrypts a string with given key
 * @param stringToEncrypt
 * @param key
 * @return
 */
Cryption Crypter::encrypt(string stringToEncrypt, long long int key) {
    vector<string> stringToEncryptSplit = StringModifier::splitString(std::move(stringToEncrypt), BLOCK_SIZE);
    Cryption enc = Crypter::encryptString(stringToEncryptSplit, key);

    return enc;
}

/**
 * Method that encrypts a Cryption with given key
 * @param c
 * @param key
 * @return
 */
Cryption Crypter::encrypt(Cryption &c, long long int key) {
    Cryption enc = Crypter::encryptString(c, key);
    return enc;
}

/**
 * Method that decrypts vector of stirngs with block size and returns Cryption
 * @param stringToDecrypt
 * @param lengths
 * @param key
 * @return
 */
Cryption Crypter::decrypt(string stringToDecrypt,vector<int> lengths, long long int key) {
    vector<string> str = StringModifier::splitString(std::move(stringToDecrypt), BLOCK_SIZE);

    return decryptString(str, std::move(lengths),key);
}

/**
 * Decrypts a Cryption with given key
 * @param cryption
 * @param key
 * @return
 */
Cryption Crypter::decrypt(Cryption &cryption, long long int key) {
    return decryptString(cryption, key);
}

/**
 * Encrypts a string with key as char array
 * @param stringToEncrypt
 * @param key
 * @return
 */
Cryption Crypter::encrypt(string stringToEncrypt, unsigned char* key) {
    vector<string> stringToEncryptSplit = StringModifier::splitString(std::move(stringToEncrypt), BLOCK_SIZE);
    Cryption enc = Crypter::encryptString(stringToEncryptSplit, key);

    return enc;
}

/**
 * Decrypts a Cryption with key as char array
 * @param cryption
 * @param key
 * @return
 */
Cryption Crypter::decrypt(Cryption &cryption, unsigned char* key) {
    return decryptString(cryption, key);
}
