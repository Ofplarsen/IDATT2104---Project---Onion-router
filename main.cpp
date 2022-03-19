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
    InputNode inp1;
    ExitNode exn1;
    n1.nextNode = &exn1;
    //inp1.initialize_server_socket(8081);
    std::thread t1(&InputNode::initialize_server_socket, &inp1, "8081");
    std::thread t2(&Node::initialize_server_socket, &inp1, "8087", "8080");
    std::thread t3(&ExitNode::initialize_server_socket, &exn1, "8080");
    //n1.initialize_server_socket("8080");

    t1.join();
    t2.join();
    t3.join();
    return 0;
}