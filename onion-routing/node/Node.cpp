//
// Created by xray2 on 14/03/2022.
//

#include "Node.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <regex>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

void Node::initialize_server_socket(const char *port_nr) {
    WSAData wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    std::cout << "Initializing winosck..." << std::endl;
    if (iResult != 0) {
        std::cout << "Something went wrong " << WSAGetLastError() << std::endl;
        return;
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
        return;
    }

    SOCKET ListenSocket = INVALID_SOCKET;

    // Create a SOCKET for the server to listen for client connections
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int) result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    freeaddrinfo(result);

    if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "Listen failed with error: %ld\n", WSAGetLastError() );
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    SOCKET ClientSocket = INVALID_SOCKET;

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    #define DEFAULT_BUFLEN 512

    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    int iSendResult;
    int iStart;
    string initial_user_req;
    string user_url;
    //regex url_regex("^www\\.?[a-z0-9]+([\\-\\.]{1}[a-z0-9]+)*\\.[a-z]{2,5}(:[0-9]{1,5})?(\\/.*)?$"); //Does not work as expected
    regex url_regex2("[www\\.?a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)");
    smatch match;
    string get_req_domain;

    // Receive until the peer shuts down the connection
    do {
        do {
        iStart = recv(ClientSocket, recvbuf, recvbuflen, 0); //initial request from prev/client
        printf("Bytes received: %d\n", iStart);
        initial_user_req += string(recvbuf).substr(0, iStart);
        iResult = iStart;
        } while(iStart == 512); //TODO this might not be very secure. What if user sends some data in smaller packages than 512? Or exactly 512 x times? That will break the program, recv blocks for ever.

        //Looking for domain name in user request from browser
        regex_search(initial_user_req, match, url_regex2);
        for(auto x: match)std::cout << x << "\n";
        std::cout << initial_user_req << "\n";
        int user_arg_end;
        bool contains_path = false;
        int path_length = 0;
        for(user_arg_end = 5; initial_user_req[user_arg_end] != ' '; user_arg_end++){
            if(initial_user_req[user_arg_end] == '/') contains_path = true;
            if(contains_path) path_length++;
        }
        printf("%d",path_length);
        string domain_name = initial_user_req.substr(5, user_arg_end - 5 - path_length); //Domain name always starts at position 5 in get request, then goes up to the length of the entire URL - path size - offset
        std::cout << domain_name << "\n";
        string path;
        if(contains_path) path = initial_user_req.substr(1 + user_arg_end - path_length, path_length); //Path starts right after /, therefore +1
        std::cout << path << "\n";

        //constructing a get request that the node will send to socket on internet
        //TODO memory????????qatlnks1.html
        const char *get_req_pre = "GET / HTTP/1.1\r\nHost: ";
        get_req_domain = match[0].str(); //Extracting domain name from the regex matches
        const char *get_req_post = "\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:98.0) Gecko/20100101 Firefox/98.0\r\nConnection: close\r\n\r\n";
        string get_req = get_req_pre + get_req_domain + get_req_post;
        const char *get_req_ptr = get_req.c_str();
        std::cout << get_req << std::endl;
        //TODO memory????????

        //getting IP address from domain sent in by user
        hostent *webDomain = gethostbyname(get_req_domain.c_str());
        in_addr *addr; //To get  char  version of ip: inet_ntoa(*addr)
        for(int i = 0; ; ++i) //Purposefully left the second condition out, because we will be testing for it inside the loop.
        {
            char *temp = webDomain->h_addr_list[i];
            if(temp == NULL) //we reached the end of the list
                break; //exit the loop.

            addr = reinterpret_cast<in_addr*>(temp);
        }

        const char *testSend = "GET /qatfaq1.html HTTP/1.1\r\n"
                               "Host: www.softwareqatest.com\r\n"
                               "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:98.0) Gecko/20100101 Firefox/98.0\r\n"
                               "Connection: close\r\n\r\n";

        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            SOCKET test = getSocket(inet_ntoa(*addr), "80");
            // Echo the buffer back to the sender
            iSendResult = send(test, get_req_ptr, (int) strlen(get_req_ptr), 0); //forwarding received message to next/server
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return;
            }

            iResult = shutdown(test, SD_SEND); //doesnt send more data
            if (iResult == SOCKET_ERROR) { //OBS OBS
                printf("shutdown failed: %d\n", WSAGetLastError());
                closesocket(test);
                WSACleanup();
                return;
            }


            string result;
            printf("Bytes sent: %d\n", iSendResult);
            do {
                iResult = recv(test, recvbuf, recvbuflen, 0);
                if (iResult > 0) {
                    printf("Bytes received: %d\n", iResult);
                    char chars[iResult + 1];
                    memcpy(chars, recvbuf, iResult);
                    chars[iResult] = '\0';
                    result += chars;
                }
                else if (iResult == 0)
                    printf("Connection closed\n");
                else
                    printf("recv failed: %d\n", WSAGetLastError());
            } while (iResult > 0);

            iSendResult = send(ClientSocket, result.c_str(), result.size(), 0); //Sender til client
            printf("final %d", iSendResult);




            /*char chars[recvbuflen + 1];
            memcpy(chars, recvbuf, recvbuflen);
            chars[recvbuflen] = '\0';       // Null-terminate the string

            std::cout << chars;*/

        } else if (iResult == 0)
            printf("Connection closing...\n");
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
    WSACleanup();

    return;
}

SOCKET Node::getSocket(const char *ip, const char *port) {
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

#define DEFAULT_TLS_PORT = "433"

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


