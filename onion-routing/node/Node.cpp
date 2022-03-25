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
    do {

        string initial_user_req;
        do {
            iStart = recv(ClientSocket, recvbuf, recvbuflen, 0); //initial request from prev/client
            printf("Bytes received: %d\n", iStart);
            cout << WSAGetLastError() << endl;
            cout<< "Received from Main: " <<recvbuf<<endl;
            //cout << "\nSubstring created: \n" << string(recvbuf).substr(0, iStart) << endl;
            initial_user_req.append(string(recvbuf, iStart).substr(0, iStart));
            iResult = iStart;
        } while(iStart == 512); //TODO this might not be very secure. What if user sends some data in smaller packages than 512? Or exactly 512 x times? That will break the program, recv blocks for ever.
        //cout << "Received from prev: " << initial_user_req << "\n" << endl;

        if (iResult > 0) {
            size_t end;
            string info;
            printf("Bytes received: %d\n", iResult);

            SOCKET web_page_socket = getConnectSocket(connectIp, connectPort);
            cout << "\nAs whole string: \n" << initial_user_req << endl;
            iResult = iStart;
            //cout << initial_user_req << endl;
            //Looking for domain name and path in user request from browser. Test webpage input www.softwareqatest.com/qatfaq2.html
//        vector<string> parsed = parse_initial_request(initial_user_req); //Contains domain_name and path
//
//        //constructing a get request that the node will send to socket on internet
//        string getReq = construct_get_request(parsed.at(0), parsed.at(1));
            end = initial_user_req.find("\r\n");
            if(end != string::npos){
                info = initial_user_req.substr(0, end);
                initial_user_req = initial_user_req.substr(end+2, initial_user_req.length());
            }
            cout<< "\nWithout Header: \n" << initial_user_req << endl;
            vector<int> sizes = split(info);
            cout << sizes[0] << endl;
            cout << sizes[1] << endl;
            cout << sizes[2] << endl;
            initial_user_req = initial_user_req.substr(0, sizes[0]*sizes[1]+sizes[2]); //TODO MEM?

            cout<< initial_user_req<< endl;
            Cryption cre = StringModifier::splitString(initial_user_req);
            cout << "cre succ" << endl;
            cre.strings_len = StringModifier::getVector(sizes[0],sizes[1],sizes[2]);
            cout << cre.strings_len.size() << endl;
            cout << cre.res.size()<< endl;

            cout << "string len suc" << endl;
            string decrypted = decrypt(cre);
            cout<<decrypted<<endl;
            string returnVal = info+"\r\n" += decrypted;
            cout << returnVal << endl;
            iSendResult = send(web_page_socket, returnVal.c_str(), returnVal.length(), 0); //forwarding received message to next/server
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
            string receiveRequest;
            size_t end1;
            string info1;
            do {
                iResult = recv(web_page_socket, recvbuf, recvbuflen, 0); //Receiving from nextNode
                receiveRequest+=string(recvbuf).substr(0, iResult);


                if (iResult > 0) {
                    printf("Bytes received: %d.\n", iResult);

                }
                else if (iResult == 0)
                    printf("Connection closed\n");
                else
                    printf("recv failed: %d\n", WSAGetLastError());
            } while (iResult > 0);

            end1 = receiveRequest.find("\r\n");
            if(end1 != string::npos){
                info1 = receiveRequest.substr(0, end1);
                receiveRequest = receiveRequest.substr(end1+2, receiveRequest.length());
            }
            vector<int> sizes1 = split(info1);
            receiveRequest = receiveRequest.substr(0, sizes1[0]*sizes1[1]+sizes1[2]); //TODO MEM?

            Cryption cre1 = StringModifier::splitString(receiveRequest);


            string enc = encrypt(receiveRequest);
            string returnVal1;
            returnVal1 = info1 += enc;

            iSendResult = send(ClientSocket, returnVal1.c_str(), returnVal1.length(), 0); //Sending back to prevNode immediately
            //Sending back to prevNode immediately
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                printf("HAPPENED IN INPUTNODE PLACE 2");
                closesocket(ClientSocket);
                WSACleanup();
                return;
            }
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

void Node::receiveAndSend() {

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
            size_t end;
            string info;
            printf("Bytes received: %d\n", iResult);

            SOCKET web_page_socket = getConnectSocket(connectIp, connectPort);

            end = initial_user_req.find("\r\n");
            if(end != string::npos){
                info = initial_user_req.substr(0, end);
                initial_user_req = initial_user_req.substr(end+2, initial_user_req.length());
            }
            vector<int> sizes = split(info);
            initial_user_req = initial_user_req.substr(0, sizes[0]*sizes[1]+sizes[2]); //TODO MEM?


            Cryption cre = StringModifier::splitString(initial_user_req);

            cre.strings_len = StringModifier::getVector(sizes[0],sizes[1],sizes[2]);

            string decrypted = decrypt(cre);
            string returnVal;
            returnVal = info += decrypted;

            iSendResult = send(web_page_socket, returnVal.c_str(), returnVal.length(), 0); //forwarding received message to next/server
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
            string receiveRequest;
            size_t end1;
            string info1;
            do {
                iResult = recv(web_page_socket, recvbuf, recvbuflen, 0); //Receiving from nextNode
                receiveRequest+=string(recvbuf).substr(0, iResult);


                if (iResult > 0) {
                    printf("Bytes received: %d.\n", iResult);

                }
                else if (iResult == 0)
                    printf("Connection closed\n");
                else
                    printf("recv failed: %d\n", WSAGetLastError());
            } while (iResult > 0);

            end1 = receiveRequest.find("\r\n");
            if(end1 != string::npos){
                info1 = receiveRequest.substr(0, end1);
                receiveRequest = receiveRequest.substr(end1+2, receiveRequest.length());
            }
            vector<int> sizes1 = split(info1);
            receiveRequest = receiveRequest.substr(0, sizes1[0]*sizes1[1]+sizes1[2]); //TODO MEM?

            Cryption cre1 = StringModifier::splitString(receiveRequest);


            string enc = encrypt(receiveRequest);
            string returnVal1;
            returnVal1 = info1 += enc;

            iSendResult = send(ClientSocket, returnVal1.c_str(), returnVal1.length(), 0); //Sending back to prevNode immediately
            //Sending back to prevNode immediately
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                printf("HAPPENED IN INPUTNODE PLACE 2");
                closesocket(ClientSocket);
                WSACleanup();
                return;
            }
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
    cout << decryptKey.secretKey << endl;
    Cryption enc = Crypter::encrypt(message,decryptKey.secretKey);
    string encrypted;
    for(int i = 0; i < enc.getRes().size(); i++){
        for(int y = 0; y < enc.strings_len[i]; y++){
            encrypted += enc.getRes()[i][y];
        }
    }
    return encrypted;
}

Cryption Node::encryptC(string message) {
    return Crypter::encrypt(message,decryptKey.secretKey);
}

Cryption Node::encryptC(Cryption &c) {
    return Crypter::encrypt(c,decryptKey.secretKey);
}

string Node::decrypt(Cryption &message) {
    long long int key = decryptKey.secretKey;
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
    return Crypter::decrypt(message,length, encryptKey.secretKey);
}

vector<int> Node::split(string s){
    vector<int> info;
    std::string delimiter = "|";

    size_t pos = 0;
    std::string token = "";
    int x = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(token.length()+x, pos);
        info.push_back(stoi(token));

        if(x==4)
            break;

        if(x == 1){
            x = 4;
        }else if(x == 0){
            x = 1;
        }

    }
    cout << "split succ" << endl;
    return info;
}

Node::Node() {

}

void Node::printError(){
    cout << this->connectPort << endl;
    cout << this->listenPort << endl;
    cout << this->connectIp << endl;
}