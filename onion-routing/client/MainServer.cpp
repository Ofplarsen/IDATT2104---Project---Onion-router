//
// Created by xray2 on 14/03/2022.
//

#include "MainServer.h"
#include "../security/diffie-helman/Handshake.h"
#include "../security/string-modifier/StringModifier.h"
bool MainServer::generateKeys(){
    for(auto it = begin(userNodes); it != end(userNodes)-1; it++){
        long long int num = StringModifier::BN2LLI(Handshake::doHandshake(it->decryptKey, std::next(it)->encryptKey));
        keys.push_back(num);
    }

}

Cryption MainServer::encrypt(string text){
    vector<Cryption> c;
    c.push_back(userNodes[0].encryptC(text));
    if(userNodes.size() == 1)
        return c[0];
    for(int i = 1; i < userNodes.size()-1; i++){
        c.push_back(userNodes[i].encryptC(c[i-1]));
    }

    return c[userNodes.size()-1];
}

string MainServer::decrypt(Cryption &c) {
    Cryption text;
    vector<Cryption> cr;
    vector<string> s;
    cr.push_back(c);
    for(int i = userNodes.size(); i > 0; i--){
        text = userNodes[i].decrypt(cr,);
        cr.push_back(text);
    }
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

