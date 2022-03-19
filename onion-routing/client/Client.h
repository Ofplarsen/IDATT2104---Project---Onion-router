//
// Created by xray2 on 14/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_CLIENT_H
#define IDATT2104___PROJECT___ONION_ROUTER_CLIENT_H

#include "vector"
#include "../node/Node.h"
using namespace std;
class Client {
private:
    vector <Key> keys;
    Node node; //Byttes ut med socket
public:

    void generateKeys(){

    }
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_CLIENT_H
