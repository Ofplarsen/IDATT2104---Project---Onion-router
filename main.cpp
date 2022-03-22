#include <iostream>
#include "onion-routing/node/Node.h"
#include "onion-routing/security/diffie-helman/SC.h"
#include "onion-routing/security/aes/Crypter.h"
#include "onion-routing/security/model/Cryption.h"
#include "onion-routing/security/string-modifier/StringModifier.h"
#include "onion-routing/client/MainServer.h"
#include "onion-routing/node/InputNode.h"
#include "onion-routing/node/ExitNode.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <ctime>
#include <thread>
#include <openssl/err.h>
#include <openssl/evp.h>

int main(int argc, char const *argv[]){

    MainServer main;

    vector<Node> nodes = main.getUserNodes();

    std::string text = "Great lets test this bread wtf is asd asd a sd sdddd ";
    string encrypted;
    string decrypted;
    vector<Cryption> c;
    encrypted = nodes[0].encrypt(text);
    c.push_back(nodes[0].encryptC(text));

    decrypted = nodes[1].decrypt(c[0]);
    cout << std::hex <<decrypted << endl;

    cout << "\n\n"<< endl;

    encrypted = nodes[1].encrypt(encrypted);
    c.push_back(nodes[1].encryptC(c[0]));

    decrypted = nodes[2].decrypt(c[1]);
    cout << std::hex << decrypted << endl;
    decrypted = nodes[1].decrypt(c[0]);
    cout << std::hex <<decrypted << endl;

    cout << "\n\n"<< endl;



    encrypted = nodes[2].encrypt(encrypted);
    c.push_back(nodes[2].encryptC(c[1]));
    cout <<std::hex << encrypted << endl;
    decrypted = nodes[3].decrypt(c[2]);
    cout << std::hex << decrypted << endl;
    decrypted = nodes[2].decrypt(c[1]);
    cout << std::hex << decrypted << endl;
    decrypted = nodes[1].decrypt(c[0]);
    cout << std::hex <<decrypted << endl;

    c.pop_back();
    c.pop_back();
    c.pop_back();
    encrypted = nodes[0].encrypt(text);
    c.push_back(nodes[0].encryptC(text));
    encrypted = nodes[1].encrypt(encrypted);
    c.push_back(nodes[1].encryptC(c[0]));
    encrypted = nodes[2].encrypt(encrypted);
    c.push_back(nodes[2].encryptC(c[1]));
    cout <<std::hex << encrypted << endl;
    decrypted = nodes[3].decrypt(c[2]);
    cout << std::hex << decrypted << endl;
    decrypted = nodes[2].decrypt(c[1]);
    cout << std::hex << decrypted << endl;
    decrypted = nodes[1].decrypt(c[0]);
    cout << std::hex <<decrypted << endl;


    c.pop_back();
    c.pop_back();
    c.pop_back();
    encrypted = nodes[0].encrypt(text);
    c.push_back(nodes[0].encryptC(text));
//    encrypted = nodes[1].encrypt(encrypted);
//    c.push_back(nodes[1].encryptC(c[0]));
//    encrypted = nodes[2].encrypt(encrypted);
//    c.push_back(nodes[2].encryptC(c[1]));
//    cout <<std::hex << encrypted << endl;
//    decrypted = nodes[3].decrypt(c[2]);
//    cout << std::hex << decrypted << endl;
//    decrypted = nodes[2].decrypt(c[1]);
//    cout << std::hex << decrypted << endl;
    decrypted = nodes[1].decrypt(c[0]);
    cout << std::hex <<decrypted << endl;
//    char recvbuf[512];
//    int recvbuflen = 512;
//    string test = "";
//    SOCKET receive = nodes[0].getListenSocket("8081");
//    //SOCKET ClientSocket = accept(receive, NULL, NULL);
//    if (ClientSocket == INVALID_SOCKET) {
//        printf("accept failed: %d\n", WSAGetLastError());
//        closesocket(receive);
//        WSACleanup();
//        return -1;
//    }
//    SOCKET sender = nodes[0].getConnectSocket("192.168.10.100", "8081");
//    int res = send(sender,encrypted.c_str(), encrypted.length(), 0);
//    cout << encrypted.c_str() << endl;
//    cout << encrypted << endl;
//    int res2 =  recv(receive, recvbuf, recvbuflen, 0);
//    if (res2 == SOCKET_ERROR) {
//        printf("send failed: %d\n", WSAGetLastError());
//        closesocket(receive);
//        closesocket(sender);
//        WSACleanup();
//        return 0;
//    }
//    test += string(recvbuf).substr(0, res2); //Gathering user request in a string
//    cout << test << endl << endl;
//    closesocket(receive);
//    closesocket(sender);
//    WSACleanup();
//    Node n1;
//    InputNode inp1;
//    ExitNode exn1;
//    n1.nextNode = &exn1;
    //inp1.initialize_server_socket(8081);
//std::thread t1(&InputNode::initialize_server_socket, &inp1, "8081");
//    std::thread t2(&Node::initialize_server_socket, &inp1, "8087", "8080");
//    std::thread t3(&ExitNode::initialize_server_socket, &exn1, "8080");
//    //n1.initialize_server_socket("8080");
//
//    t1.join();
//    t2.join();
//    t3.join();
    return 0;

    //Cryption enc = Crypter::encrypt(text, BN_get_word(nodes[0].decryptKey.getSecretKey1()));
//    string encrypted;
//    string decrypted;
//    vector<Cryption> c;
//    for(int i = 0; i < 3; i++){
//
//        encrypted = nodes[i].encrypt(text);
//        c.push_back(nodes[i].encryptC(text));
//        text = encrypted;
//
//    }
//    cout << encrypted << endl;
//    int x = 2;
//    for (int i = 3; i > 0; --i) {
//        decrypted = nodes[i].decrypt(c[x]);
//        x--;
//    }
//    cout << decrypted << endl;

//    encrypted = nodes[0].encrypt(text);
//    c.push_back(nodes[0].encryptC(text));
//    encrypted = nodes[1].encrypt(encrypted);
//    c.push_back(nodes[1].encryptC(c[0]));
//    cout <<std::hex << encrypted << endl;
//    decrypted = nodes[2].decrypt(c[1]);
//    cout << std::hex << decrypted << endl;
//    decrypted = nodes[1].decrypt(c[0]);
//    cout << std::hex <<decrypted << endl;
    return 0;
}