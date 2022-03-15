//
// Created by xray2 on 14/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_NODE_H
#define IDATT2104___PROJECT___ONION_ROUTER_NODE_H


#include <string>
#include "../key/Key.h"

using namespace std;

class Node {
private:
    Node *prevNode;
    Node *nextNode;
    Key key;
    string message;
public:
    string encrypt(string message){

    }
    string decrypt(string message){

    }
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_NODE_H
