//
// Created by xray2 on 14/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_INPUTNODE_H
#define IDATT2104___PROJECT___ONION_ROUTER_INPUTNODE_H


#include "Node.h"
#include <string>
#include <vector>
#include <winsock.h>

using namespace std;


class InputNode: public Node{
public:

    InputNode();

    InputNode(const char *listenPort, const char *connectPort, const char *connectIp);

    void initialize_server_socket(const char *listenPort, const char *connectPort, const char *connectIp);
    void receiveAndSend();
    SOCKET getConnectSocket(const char *ip, const char *port);
    SOCKET getListenSocket(const char *port_nr);
    static vector<string> parse_initial_request(string req);
    static string construct_get_request(string domain_name, string path);
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_INPUTNODE_H
