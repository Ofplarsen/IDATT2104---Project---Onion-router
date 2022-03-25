//
// Created by xray2 on 14/03/2022.
//

#include <winsock2.h>
#include <ws2tcpip.h>
#include "ExitNode.h"
#include "../security/string-modifier/StringModifier.h"

vector<string> ExitNode::parseInitialReq(string req) {
    int user_arg_end;
    bool contains_path = false;
    int path_length = 0;
    string path;
    for(user_arg_end = 5; req[user_arg_end] != ' '; user_arg_end++){
        if(req[user_arg_end] == '/') contains_path = true;
        if(contains_path) path_length++;
    }
    string domain_name = req.substr(5, user_arg_end - 5 - path_length); //Domain name always starts at position 5 in get request, then goes up to the length of the entire URL - path size - offset
    if(contains_path) path = req.substr(1 + user_arg_end - path_length, path_length - 1); //Path starts right after /, therefore +1

    vector<string> domain_and_path(2);
    domain_and_path.at(0) = domain_name;
    domain_and_path.at(1) = path;
    return domain_and_path;
}

string ExitNode::constructGetReq(string domain_name, string path) { //Constructs a modified GET request where the first line is the length of the domain and path separated by |
    string get_req = "GET /" + path + " HTTP/1.1\r\nHost: " + domain_name +
                     "\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:98.0) Gecko/20100101 Firefox/98.0\r\nConnection: close\r\n\r\n";
    return get_req;
}

void ExitNode::initialize_server_socket(const char *listenPort) {

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

    int recvbuflen = DEFAULT_BUFLEN;
    int iSendResult;
    int iStart;

    // Receive until the peer shuts down the connection
    char recvbuf[DEFAULT_BUFLEN];
    do {
        size_t end;
        string info;
        string initial_user_req;
        do {
            iStart = recv(ClientSocket, recvbuf, recvbuflen, 0); //initial request from prev/client
            printf("Bytes received: %d\n", iStart);
            cout << WSAGetLastError() << endl;
            cout<< "Received from Last node: " <<recvbuf<<endl;
            //cout << "\nSubstring created: \n" << string(recvbuf).substr(0, iStart) << endl;
            initial_user_req.append(string(recvbuf, iStart).substr(0, iStart));
            iResult = iStart;
        } while(iStart == 512); //TODO this might not be very secure. What if user sends some data in smaller packages than 512? Or exactly 512 x times? That will break the program, recv blocks for ever.
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
        initial_user_req = initial_user_req.substr(0, sizes[0]*sizes[1]+sizes[2]);
        //Looking for domain name and path in user request from browser.
        vector<string> parsed = parseInitialReq(initial_user_req);
        string domain = parsed.at(0);

        //Constructing a get request that the node will send to webserever
        string getReq = constructGetReq(parsed.at(0), parsed.at(1));

        cout<<"domain"<<domain<<endl;
        cout<<"getReq"<<getReq<<endl;

        //TODO MEM?

//        cout<< initial_user_req<< endl;
//        Cryption cre = StringModifier::splitString(initial_user_req);
//        cout << "cre succ" << endl;
//        cre.strings_len = StringModifier::getVector(sizes[0],sizes[1],sizes[2]);
//        cout << cre.strings_len.size() << endl;
//        cout << cre.res.size()<< endl;

//        cout << "string len suc" << endl;
//        string decrypted = decrypt(cre);
//        cout<<decrypted<<endl;
//        string returnVal = info+"\r\n" += decrypted;
//        cout << returnVal << endl;


        //getting IP address from domain sent in by user
        hostent *webDomain = gethostbyname(domain.c_str());
        in_addr *addr; //To get  char  version of ip: inet_ntoa(*addr)
        for(int i = 0; ; ++i) //Purposefully left the second condition out, because we will be testing for it inside the loop.
        {
            char *temp = webDomain->h_addr_list[i];
            if(temp == NULL) //we reached the end of the list
                break; //exit the loop.

            addr = reinterpret_cast<in_addr*>(temp);
        }
        //cout<<inet_ntoa(*addr)<<endl;
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            SOCKET web_page_socket = getConnectSocket(inet_ntoa(*addr), "80"); //HTTP listens on port 80
            iSendResult = send(web_page_socket, getReq.c_str(), getReq.length(), 0); //forwarding received message to next/server
            if (iSendResult == SOCKET_ERROR) {
                printf("HAPPENED IN EXITNODE PLACE 1");
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
                    printf("Bytes received: %d. Bytes sent: %d\n", iResult, iSendResult);
                    receiveRequest+=string(recvbuf, iResult).substr(0, iResult);
                }
                else if (iResult == 0)
                    printf("Connection closed\n");
                else
                    printf("recv failed: %d\n", WSAGetLastError());


            } while (iResult > 0);

            cout << receiveRequest << endl;
            Cryption cre1 = StringModifier::splitString(receiveRequest);


            string enc = encrypt(receiveRequest);
            string returnVal1;
            returnVal1 = cre1.getRequestString() += enc;
            cout << returnVal1 << endl;
            iSendResult = send(ClientSocket, returnVal1.c_str(), returnVal1.length(), 0); //Sending back to prevNode immediately
            if (iSendResult == SOCKET_ERROR) {
                printf("HAPPENED IN EXITNODE PLACE 2");
                printf("send failed: %d\n", WSAGetLastError());
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


SOCKET ExitNode::getConnectSocket(const char *ip, const char *port) {
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

SOCKET ExitNode::getListenSocket(const char *port_nr) {
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

ExitNode::ExitNode() {}

ExitNode::ExitNode(const char *listenPort, const char *connectPort, const char *connectIp) : Node(listenPort,
                                                                                                  connectPort,
                                                                                                  connectIp) {}


