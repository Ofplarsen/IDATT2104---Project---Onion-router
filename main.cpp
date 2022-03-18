#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>
#include <thread>
#include "onion-routing/node/Node.h"
#include "onion-routing/node/ExitNode.h"
#include "onion-routing/node/InputNode.h"

#define PORT 8080

int main(int argc, char const *argv[])
{
    Node n1;
    ExitNode exn1;
    InputNode inp1;
    n1.nextNode = &exn1;
    const char *ptr = "8081";
    inp1.initialize_server_socket(ptr);
    //std::thread t1(inp1.initialize_server_socket, ptr);
    n1.initialize_server_socket("8080");


    return 0;
}