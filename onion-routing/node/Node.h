//
// Created by xray2 on 14/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_NODE_H
#define IDATT2104___PROJECT___ONION_ROUTER_NODE_H


#include <string>
#include <winsock.h>
#include "../security/key/Key.h"
#include "../socket/SocketGetters.h"
#include "../security/model/Cryption.h"

using namespace std;

class Node {
protected:
    const char *listenPort;
    const char *connectPort;
    const char *connectIp;
private:

    string message;
public:
    Node();
    Node(const char *listenPort, const char *connectPort, const char *connectIp){
        this->listenPort = listenPort;
        this->connectPort = connectPort;
        this->connectIp = connectIp;
    }
    Key encryptKey;
    Key decryptKey;
    string encrypt(string message);
    string decrypt(string message);
    string get_public_key();
    void send_message(string message);
    void receive_message(string message);
    void connect();

    void initialize_server_socket(const char *listenPort, const char *connectPort, const char *connectIp);
    SOCKET getConnectSocket(const char *ip, const char *port);
    SOCKET getListenSocket(const char *port_nr);


    Cryption encrypt(Cryption message);

    Cryption encryptC(string message);

    Cryption decryptC(Cryption message);

    string decrypt(Cryption &message);

    Cryption encryptC(Cryption &c);

    Cryption decryptC(vector<string> msg, vector<int> length);

    Cryption buildCryption(string message, string len);

    string buildString(Cryption &c);

    vector<int> split(string s);

    void receiveAndSend();

    void printError();
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_NODE_H
