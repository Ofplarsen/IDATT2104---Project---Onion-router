//
// Created by xray2 on 14/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_EXITNODE_H
#define IDATT2104___PROJECT___ONION_ROUTER_EXITNODE_H

#include <winsock2.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>

#define PORT 8080

#include "Node.h"
#include <vector>


class ExitNode : public Node{
private:
    int iResult{};

public:
    ExitNode();

    ExitNode(const char *listenPort, const char *connectPort, const char *connectIp);

    explicit ExitNode(const char *listenPort){
        this->listenPort=listenPort;
    }
    void sendGetRequest(const char *ip, const char *port);

    void initialize_server_socket(const char *listenPort);
    void receiveAndSend();

    SOCKET getListenSocket(const char *nr);
    SOCKET getConnectSocket(const char *ip, const char *port);
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_EXITNODE_H
