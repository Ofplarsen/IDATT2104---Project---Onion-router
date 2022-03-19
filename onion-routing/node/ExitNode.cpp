//
// Created by xray2 on 14/03/2022.
//

#include "ExitNode.h"

void ExitNode::sendGetRequest(const char *ip, const char *port) {
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return;
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
        return;
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
        return;
    }

    // Connect to server.
    iResult = ::connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    // Should really try the next address returned by getaddrinfo
    // if the connect call failed
    // But for this simple example we just free the resources
    // returned by getaddrinfo and print an error message

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return;
    }

#define DEFAULT_BUFLEN 512

    int recvbuflen = DEFAULT_BUFLEN;

    const char *sendbuf = "GET / HTTP/1.1\r\n"
                          "Host: localhost:1250\r\n"
                          "Connection: keep-alive\r\n"
                          "Cache-Control: max-age=0\r\n"
                          "sec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"99\", \"Google Chrome\";v=\"99\"\r\n"
                          "sec-ch-ua-mobile: ?0\r\n"
                          "sec-ch-ua-platform: \"Windows\"\r\n"
                          "Upgrade-Insecure-Requests: 1\r\n"
                          "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36\r\n"
                          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
                          "Sec-Fetch-Site: none\r\n"
                          "Sec-Fetch-Mode: navigate\r\n"
                          "Sec-Fetch-User: ?1\r\n"
                          "Sec-Fetch-Dest: document\r\n"
                          "Accept-Encoding: gzip, deflate, br\r\n"
                          "Accept-Language: nb-NO,nb;q=0.9,no;q=0.8,nn;q=0.7,en-US;q=0.6,en;q=0.5,de;q=0.4\r\n\r\n";

    char recvbuf[DEFAULT_BUFLEN];

    // Send an initial buffer
    iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return;
    }

    printf("Bytes Sent: %ld\n", iResult);

// shutdown the connection for sending since no more data will be sent
// the client can still use the ConnectSocket for receiving data
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return;
    }

    // Receive data until the server closes the connection
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());
    } while (iResult > 0);

    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return;
    }

    closesocket(ConnectSocket);
    WSACleanup();

}

void ExitNode::initialize_server_socket(const char *port_nr) {

    SOCKET ListenSocket = getListenSocket(port_nr); //Making a socket listen on given port

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
    int iResult;
    int iSendResult;
    int iStart;
    string initial_user_req;
    string user_url;

    // Receive until the peer shuts down the connection
    do {
        do {
            iStart = recv(ClientSocket, recvbuf, recvbuflen, 0); //initial request from prev/client
            printf("Bytes received: %d\n", iStart);
            initial_user_req += string(recvbuf).substr(0, iStart);
        } while(iStart == 512); //TODO this might not be very secure. What if user sends some data in smaller packages than 512? Or exactly 512 x times? That will break the program, recv blocks for ever.
        iResult = iStart;

        //Looking for domain name and path in user request from browser. Test webpage input www.softwareqatest.com/qatfaq2.html
        vector<string> parsed = parse_initial_request(initial_user_req);

        //constructing a get request that the node will send to socket on internet
        const char *get_req_ptr = construct_get_request(parsed.at(0), parsed.at(1)).c_str();
        std::cout << get_req_ptr << std::endl;

        //getting IP address from domain sent in by user
        /*hostent *webDomain = gethostbyname(domain_name.c_str());
        in_addr *addr; //To get  char  version of ip: inet_ntoa(*addr)
        for(int i = 0; ; ++i) //Purposefully left the second condition out, because we will be testing for it inside the loop.
        {
            char *temp = webDomain->h_addr_list[i];
            if(temp == NULL) //we reached the end of the list
                break; //exit the loop.

            addr = reinterpret_cast<in_addr*>(temp);
        }*/

        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            SOCKET web_page_socket = getConnectSocket("192.168.1.14", "8080"); //TODO fix here to change which connection to forward to
            cout << "connected to next, trying to send" << endl;
            iSendResult = send(web_page_socket, get_req_ptr, (int) strlen(get_req_ptr), 0); //forwarding received message to next/server
            if (iSendResult == SOCKET_ERROR) {
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

vector<string> ExitNode::parse_initial_request(string req) {
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

string ExitNode::construct_get_request(string domain_name, string path) { //Constructs a modified GET request where the first line is the length of the path and domain separated by |
    string get_req = to_string(path.length()) + "|" + to_string(domain_name.length()) + "\r\n" +
                     "GET /" + path + " HTTP/1.1\r\nHost: " + domain_name +
                     "\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:98.0) Gecko/20100101 Firefox/98.0\r\nConnection: close\r\n\r\n";
    return get_req;
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

