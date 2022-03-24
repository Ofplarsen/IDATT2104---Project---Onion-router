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
    void initialize_server_socket(const char *listenPort, const char *connectPort, const char *connectIp);
    vector<string> parse_initial_request(string req);
    string construct_get_request(string domain_name, string path);
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_INPUTNODE_H
