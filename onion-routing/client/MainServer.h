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
    Node node; //Byttes ut med socket
    InputNode userInputNode;
    ExitNode userExitNode;
    vector<Node> userNodes;

    bool generateKeys();

    string decrypt();
    vector <InputNode> inputNodePool; //Amount: 5
    vector <Node> nodePool; //Amount: 10
    vector <ExitNode> exitNodePool; //Amount: 5

    SOCKET server; //Client connects to this one through browser
    SOCKET forward; //Used to forward information to Nodes

    int nodeAmount; //Amount of nodes
    void initNodes();
public:
    vector<long long int> keys;
    const vector<Node> &getUserNodes() const;

    Cryption encrypt(string text);

    int start();
    static string parseGetReq(string req);
    int getNodeAmount(int min, int max);
    static string welcome();
    static string help();

    static string notFound();

    string decrypt(Cryption &c);

    string sendMessage(string message);

    string receiveMessage(Cryption &c);

    static vector<int> split(string s);

};


#endif //IDATT2104___PROJECT___ONION_ROUTER_MAINSERVER_H
