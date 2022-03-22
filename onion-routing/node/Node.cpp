//
// Created by xray2 on 14/03/2022.
//

#include "Node.h"
#include "../security/model/Cryption.h"
#include "../security/aes/Crypter.h"
#include "../security/string-modifier/StringModifier.h"
#include <iostream>



string Node::encrypt(string message) {
    cout << StringModifier::BN2LLI(decryptKey.secretKey) << endl;
    Cryption enc = Crypter::encrypt(message,StringModifier::BN2LLI(decryptKey.secretKey));
    string encrypted;
    for(int i = 0; i < enc.getRes().size(); i++){
        for(int y = 0; y < enc.strings_len[i]; y++){
            encrypted += enc.getRes()[i][y];
        }
    }
    return encrypted;
}

Cryption Node::encryptC(string message) {
    return Crypter::encrypt(message,StringModifier::BN2LLI(decryptKey.secretKey));
}

Cryption Node::encryptC(Cryption &c) {
    return Crypter::encrypt(c,StringModifier::BN2LLI(decryptKey.secretKey));
}

string Node::decrypt(Cryption &message) {
    long long int key = StringModifier::BN2LLI(encryptKey.secretKey);
    cout << key << endl;
    Cryption dec = Crypter::decrypt(message, key);
    string decrypted;
    for(int i = 0; i < dec.getRes().size(); i++){
        for(int y = 0; y < dec.strings_len[i]; y++){
            decrypted += dec.getRes()[i][y];
        }
    }
    return decrypted;
}

Cryption Node::decryptC(vector<string> msg, vector<int> length) {
    return Crypter::decrypt(message,length, StringModifier::BN2LLI(encryptKey.secretKey));
}
