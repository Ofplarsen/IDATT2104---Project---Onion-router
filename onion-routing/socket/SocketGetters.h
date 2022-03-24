//
// Created by willi on 21.03.2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_SOCKETGETTERS_H
#define IDATT2104___PROJECT___ONION_ROUTER_SOCKETGETTERS_H


#include <winsock.h>

class SocketGetters {
public:
    static SOCKET getConnectSocket(const char *ip, const char *port);
    static SOCKET getListenSocket(const char *port_nr);

    static char *getLocalhostIP();
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_SOCKETGETTERS_H
