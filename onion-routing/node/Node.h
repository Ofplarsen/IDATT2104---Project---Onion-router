//
// Created by xray2 on 14/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_NODE_H
#define IDATT2104___PROJECT___ONION_ROUTER_NODE_H


#include <string>
#include <winsock.h>
#include "../security/key/Key.h"
#include "../socket/SocketGetters.h"

using namespace std;

class Node {
private:

    Key private_key;
    Key public_key;
    string message;

    const char *listenPort;
    const char *connectPort;
    const char *connectIp;
public:
    Node *prevNode;
    Node *nextNode;
    string encrypt(string message);
    string decrypt(string message);
    string get_public_key();
    void send_message(string message);
    void receive_message(string message);
    void connect();

    void initialize_server_socket(const char *listenPort, const char *connectPort, const char *connectIp);
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_NODE_H
