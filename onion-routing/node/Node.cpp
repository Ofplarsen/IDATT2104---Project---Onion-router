//
// Created by xray2 on 14/03/2022.
//

#include <winsock2.h>
#include "Node.h"
#include "../security/model/Cryption.h"
#include "../security/aes/Crypter.h"
#include "../security/string-modifier/StringModifier.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <regex>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

void Node::initialize_server_socket(const char *listenPort, const char *connectPort, const char *connectIp) {

    SOCKET ListenSocket = getListenSocket(listenPort); //Making a socket listen on given port

    SOCKET ClientSocket = INVALID_SOCKET;

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    cout << "PrevSocket connected" << endl;
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    #define DEFAULT_BUFLEN 512

    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    int iResult;
    int iSendResult;
    int iStart;
    string initial_user_req;

    // Receive until the peer shuts down the connection
    char recvbuf[DEFAULT_BUFLEN];
    do {

        string initial_user_req;
        do {
        iStart = recv(ClientSocket, recvbuf, recvbuflen, 0); //Initial request from prev/client
        printf("Bytes received: %d\n", iStart);
        initial_user_req += string(recvbuf).substr(0, iStart); //Gathering user request in a string
        //cout << recvbuf << endl;
        iResult = iStart;
        } while(iStart == 512); //TODO this might not be very secure. What if user sends some data in smaller packages than 512? Or exactly 512 x times? That will break the program, recv blocks for ever.
        //cout << "Received from prev: " << initial_user_req << "\n" << endl;

        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            SOCKET web_page_socket = getConnectSocket(connectIp, connectPort);
            iSendResult = send(web_page_socket, initial_user_req.c_str(), initial_user_req.length(), 0); //forwarding received message to next/server
            if (iSendResult == SOCKET_ERROR) {
                printf("HAPPENED IN NODE PLACE 1");
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return;
            }
            printf("Bytes sent: %d\n", iSendResult); // Echo the buffer back to the sender

            //TODO can we move this down as with the shutdown of ClientSocket?
            iResult = shutdown(web_page_socket, SD_SEND); //doesnt send more data
            if (iResult == SOCKET_ERROR) { //OBS OBS
                printf("shutdown failed: %d\n", WSAGetLastError());
                closesocket(web_page_socket);
                WSACleanup();
                return;
            }

            do {
                iResult = recv(web_page_socket, recvbuf, recvbuflen, 0); //Receiving from nextNode
                iSendResult = send(ClientSocket, recvbuf, recvbuflen, 0); //Sending back to prevNode immediately
                if (iSendResult == SOCKET_ERROR) {
                    printf("HAPPENED IN NODE PLACE 2");
                    printf("send failed: %d\n", WSAGetLastError());
                    closesocket(ClientSocket);
                    WSACleanup();
                    return;
                }
                if (iResult > 0) {
                    printf("Bytes received: %d. Bytes sent: %d\n", iResult, iSendResult);

                }
                else if (iResult == 0)
                    printf("Connection closed\n");
                else
                    printf("recv failed: %d\n", WSAGetLastError());
            } while (iResult > 0);

            printf("final %d\n", iSendResult);
            closesocket(web_page_socket);

        }
        else if (iResult == 0) {
            printf("Connection closing...\n");
        }
        else {
            printf("recv failed: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return;
        }

    } while (iResult > 0);

    // shutdown the send half of the connection since no more data will be sent
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return;
    }

    // cleanup
    closesocket(ClientSocket);
    closesocket(ListenSocket);
    WSACleanup();
}

SOCKET Node::getConnectSocket(const char *ip, const char *port) {
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return NULL;
    }

    struct addrinfo *result = NULL,
            *ptr = NULL,
            hints;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;


    // Resolve the server address and port
    iResult = getaddrinfo(ip, port, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return NULL;
    }

    SOCKET ConnectSocket = INVALID_SOCKET;

    // Attempt to connect to the first address returned by
    // the call to getaddrinfo
    ptr=result;

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                           ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return NULL;
    }

    // Connect to server.
    iResult = ::connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    return ConnectSocket;
}

SOCKET Node::getListenSocket(const char *port_nr){
    WSAData wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    std::cout << "Initializing winosck..." << std::endl;
    if (iResult != 0) {
        std::cout << "Something went wrong " << WSAGetLastError() << std::endl;
        return NULL;
    }

    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    iResult = getaddrinfo(NULL, port_nr, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return NULL;
    }

    SOCKET ListenSocket = INVALID_SOCKET;

    // Create a SOCKET for the server to listen for client connections
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return NULL;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int) result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return NULL;
    }

    freeaddrinfo(result);

    if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "Listen failed with error: %ld\n", WSAGetLastError() );
        closesocket(ListenSocket);
        WSACleanup();
        return NULL;
    }

    return ListenSocket;
}


string Node::buildString(Cryption &c){
    string res = StringModifier::cryptionToString(c);
    res += "|" + c.getRequestString();
}

Cryption Node::buildCryption(string message, string len){
    vector<string> strings = StringModifier::splitString(message, 32);
    vector<unsigned char*> strings2;
    vector<int> string_len;
    int numberOfBlocks;
    string numblocks;
    for(int i = 0; i < len.length(); i++){
        if(len.at(i) == '|')
            break;
        numblocks += (len.at(i));
    }

    string lastBlock = numblocks.substr(numblocks.length()+4,len.length());
    int lastBlockI = stoi(lastBlock);

    numberOfBlocks = stoi(numblocks);

    for(int i = 0; i < numberOfBlocks; i++){
        string_len.emplace_back(32);
        strings2.emplace_back(StringModifier::convertToCharArray(strings[i]));
    }

    string_len.emplace_back(lastBlockI);

    return {strings2, string_len};
}

string Node::encrypt(string message) {
    cout << StringModifier::BN2LLI(decryptKey.secretKey) << endl;
    Cryption enc = Crypter::encrypt(message,StringModifier::BN2LLI(decryptKey.secretKey));
    string encrypted;
    for(int i = 0; i < enc.getRes().size(); i++){
        for(int y = 0; y < enc.strings_len[i]; y++){
            encrypted += enc.getRes()[i][y];
        }
    }
    return encrypted;
}

Cryption Node::encryptC(string message) {
    return Crypter::encrypt(message,StringModifier::BN2LLI(decryptKey.secretKey));
}

Cryption Node::encryptC(Cryption &c) {
    return Crypter::encrypt(c,StringModifier::BN2LLI(decryptKey.secretKey));
}

string Node::decrypt(Cryption &message) {
    long long int key = StringModifier::BN2LLI(encryptKey.secretKey);
    cout << key << endl;
    Cryption dec = Crypter::decrypt(message, key);
    string decrypted;
    for(int i = 0; i < dec.getRes().size(); i++){
        for(int y = 0; y < dec.strings_len[i]; y++){
            decrypted += dec.getRes()[i][y];
        }
    }
    return decrypted;
}

Cryption Node::decryptC(vector<string> msg, vector<int> length) {
    return Crypter::decrypt(message,length, StringModifier::BN2LLI(encryptKey.secretKey));
}
