//
// Created by xray2 on 14/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_MAINSERVER_H
#define IDATT2104___PROJECT___ONION_ROUTER_MAINSERVER_H

#include "vector"
#include "../node/Node.h"
#include "map"
using namespace std;
class MainServer {
private:
    vector<long long int> keys;
    Node node; //Byttes ut med socket
    vector<Node> userNodes;

    bool generateKeys();

public:
    MainServer();

    const vector<Node> &getUserNodes() const;

};


#endif //IDATT2104___PROJECT___ONION_ROUTER_MAINSERVER_H
