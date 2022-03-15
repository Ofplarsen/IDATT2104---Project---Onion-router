//
// Created by xray2 on 14/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_EXITNODE_H
#define IDATT2104___PROJECT___ONION_ROUTER_EXITNODE_H

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>

#define PORT 8080

#include "Node.h"

class ExitNode : public Node{
private:
    int iResult;

public:
    void sendGetRequest(const char *ip, const char *port);
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_EXITNODE_H