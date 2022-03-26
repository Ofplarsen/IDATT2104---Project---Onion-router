//
// Created by xray2 on 14/03/2022.
//

#include <winsock2.h>
#include "Node.h"
#include "../security/aes/Crypter.h"
#include "../security/string-modifier/StringModifier.h"
#include <iostream>
#include <regex>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

/**
 * Initializes a Node that receives and sends data to the Nodes that this one is connected.
 * @param listenPort port which the Node listens for connections on
 * @param connectPort port used to connect to next Node
 * @param connectIp IP used to connect to next Node
 */
void Node::initialize_server_socket(const char *listenPort, const char *connectPort, const char *connectIp) {

    SOCKET ListenSocket = SocketGetters::getListenSocket(listenPort); //Making a socket listen on given port

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
            initial_user_req.append(string(recvbuf, iStart).substr(0, iStart));
            iResult = iStart;
        } while(iStart == 512); //TODO this might not be very secure. What if user sends some data in smaller packages than 512? Or exactly 512 x times? That will break the program, recv blocks for ever.

        if (iResult > 0) {
            size_t end;
            string info;
            printf("Bytes received: %d\n", iResult);

            SOCKET web_page_socket = SocketGetters::getConnectSocket (connectIp, connectPort);
            cout << "\nAs whole string: \n" << initial_user_req << endl;
            iResult = iStart;

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

                if (iResult > 0) {
                    printf("Bytes received: %d.\n", iResult);
                    receiveRequest+=string(recvbuf, iResult).substr(0, iResult);
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
            returnVal1 = info1 + "\r\n" += enc;
            cout<< "IN NODE:\n" << returnVal1 << endl;
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

/**
 * Method that encrypts a string and returns a string
 * @param message
 * @return
 */
string Node::encrypt(string message) {
    cout << encryptKey.secretKey << endl;
    Cryption enc = Crypter::encrypt(message,encryptKey.secretKey);
    string encrypted;
    for(int i = 0; i < enc.getRes().size(); i++){
        for(int y = 0; y < enc.strings_len[i]; y++){
            encrypted += enc.getRes()[i][y];
        }
    }
    return encrypted;
}

/**
 * Method that decrypts a Cryption and returns as string
 * @param message
 * @return
 */
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

/**
 * Splits info string and returns how many blocks, block size and size of last block
 * @param s string whose info you want to extract
 * @return vector containing the info
 */
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

/**
 * Basic constructor
 */
Node::Node() {}
