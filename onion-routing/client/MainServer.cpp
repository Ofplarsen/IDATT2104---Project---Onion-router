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

MainServer::MainServer() {
    Node n1;
    Node n2;
    Node n3;
    Node n4;
    userNodes.push_back(n1);
    userNodes.push_back(n2);
    userNodes.push_back(n3);
    userNodes.push_back(n4);
    generateKeys();
}

const vector<Node> &MainServer::getUserNodes() const {
    return userNodes;
}

