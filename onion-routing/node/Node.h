//
// Created by xray2 on 14/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_NODE_H
#define IDATT2104___PROJECT___ONION_ROUTER_NODE_H


#include <string>
#include <winsock.h>
#include "../security/key/Key.h"
#include "../security/model/Cryption.h"

using namespace std;

class Node {
private:
    string message;
public:
    Key encryptKey;
    Key decryptKey;
    Node *prevNode;
    Node *nextNode;
    string encrypt(string message);
    //string decrypt(Cryption message);
    string get_public_key();
    void send_message(string message);
    void receive_message(string message);
    void connect();

    void initialize_server_socket(const char *port_nr, const char *next_node_port);
    SOCKET getConnectSocket(const char *ip, const char *port);
    SOCKET getListenSocket(const char *port_nr);

    Cryption encrypt(Cryption message);

    Cryption encryptC(string message);

    Cryption decryptC(Cryption message);

    string decrypt(Cryption &message);

    Cryption encryptC(Cryption &c);
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_NODE_H
