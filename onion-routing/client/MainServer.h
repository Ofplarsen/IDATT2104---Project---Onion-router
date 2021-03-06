//
// Created by xray2 on 14/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_MAINSERVER_H
#define IDATT2104___PROJECT___ONION_ROUTER_MAINSERVER_H

#include "vector"
#include "../node/Node.h"
#include "../node/InputNode.h"
#include "../node/ExitNode.h"

using namespace std;
class MainServer {
private:
    vector <InputNode> inputNodePool; //Amount: 5
    vector <Node> nodePool; //Amount: 10
    vector <ExitNode> exitNodePool; //Amount: 5

    SOCKET server; //Client connects to this one through browser
    SOCKET forward; //Used to forward information to Nodes

    int nodeAmount; //Amount of nodes
    vector <Node> userNodes;
    vector <Key> keys;
public:
    void generateKeys();
    int start();
    string parseGetReq(string req);
    int getNodeAmount(int min, int max);
    string welcome();
    string help();
    string notFound(string command);
    void traverseNodes(string userRequest, const char* localhost, SOCKET clientConnection, char* recvbuf, int recvbuflen);
    string getLocalhostIpFromUser();
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_MAINSERVER_H
