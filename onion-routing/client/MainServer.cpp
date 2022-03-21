//
// Created by xray2 on 14/03/2022.
//

#include "MainServer.h"
#include "../security/diffie-helman/Handshake.h"
bool MainServer::generateKeys(){
    for(auto it = begin(userNodes); it != end(userNodes); it++){
        Handshake::doHandshake(it->decryptKey, std::next(it)->encryptKey);
    }
}

MainServer::MainServer() {
    Node n1;
    Node n2;
    Node n3;
    userNodes.push_back(n1);
    userNodes.push_back(n2);
    userNodes.push_back(n3);
    generateKeys();
}
