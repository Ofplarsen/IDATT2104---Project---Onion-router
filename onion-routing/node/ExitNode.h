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


#include "Node.h"
#include <vector>


class ExitNode : public Node{
private:
    int iResult;

public:
    void sendGetRequest(const char *ip, const char *port);
    void initialize_server_socket(const char *listenPort);
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_EXITNODE_H
