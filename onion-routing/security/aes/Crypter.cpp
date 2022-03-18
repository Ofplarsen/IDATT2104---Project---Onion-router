//
// Created by xray2 on 16/03/2022.
//
#include <math.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include "Crypter.h"

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
        perror("EVP_EncryptFinal_ex");
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

