#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>
#include "onion-routing/node/Node.h"
#include "onion-routing/node/ExitNode.h"

#define PORT 8080

int main(int argc, char const *argv[])
{
    Node n1;
    ExitNode exn1;
    n1.nextNode = &exn1;
    const char *ptr = "8081";
    n1.initialize_server_socket(ptr);


    return 0;
}