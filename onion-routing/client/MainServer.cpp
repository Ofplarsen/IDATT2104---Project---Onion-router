//
// Created by xray2 on 14/03/2022.
//

#include <iostream>
#include "MainServer.h"
#include "../security/diffie-helman/Handshake.h"
#include "../security/string-modifier/StringModifier.h"
#include "../security/aes/Crypter.h"
bool MainServer::generateKeys(){
    for(auto it = begin(userNodes); it != end(userNodes)-1; it++){
        long long int num = StringModifier::BN2LLI(Handshake::doHandshake(it->decryptKey, std::next(it)->encryptKey));
        keys.push_back(num);
    }

}

Cryption MainServer::encrypt(string text){
    vector<Cryption> c;
    c.push_back(Crypter::encrypt(text, keys[0]));
    for(int i = 1; i < userNodes.size()-1; i++){
        c.push_back(Crypter::encrypt(c[i-1], keys[i]));
    }

    return c[userNodes.size()-2];
}

string MainServer::decrypt(Cryption &c) {
    Cryption text;
    vector<Cryption> cr;
    vector<string> s;
    cr.push_back(c);
    int x = 0;
    for(int i = userNodes.size()-2; i >= 0; i--){
        text = Crypter::decrypt(cr[x], keys[i]);
        cr.push_back(text);
        x++;
    }
    return StringModifier::cryptionToString(cr[userNodes.size()-1]);
}

MainServer::MainServer(int numberOfNodes) {
    for (int i = 0; i < numberOfNodes; i++) {
        Node n;
        userNodes.push_back(n);
    }
    generateKeys();
}

const vector<Node> &MainServer::getUserNodes() const {
    return userNodes;
}

void MainServer::sendMessage(string message){
    Cryption encrypted = encrypt(message);
}

void MainServer::receiveMessage(Cryption &c){
    string t = decrypt(c);
    cout << t << endl;
}