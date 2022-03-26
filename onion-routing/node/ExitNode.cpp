//
// Created by xray2 on 14/03/2022.
//

#include <winsock2.h>
#include "ExitNode.h"

/**
 * Initializes an ExitNode which listens on a given port. The data received from previous Nodes is used to figure out
 * which website ExitNode should redirect the data to, and sends it to the given HTTP website. The response is then sent
 * bak to the previous Nodes.
 * @param listenPort port used to listen for connections
 */
void ExitNode::initialize_server_socket(const char *listenPort) {

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

    int recvbuflen = DEFAULT_BUFLEN;
    int iSendResult;
    int iStart;
    int iResult;

    // Receive until the peer shuts down the connection
    char recvbuf[DEFAULT_BUFLEN];
    do {

        string initial_user_req;
        do {
            iStart = recv(ClientSocket, recvbuf, recvbuflen, 0); //Initial request from prev/client
            printf("Bytes received: %d\n", iStart);
            initial_user_req += string(recvbuf).substr(0, iStart); //Gathering user request in a string
            iResult = iStart;
        } while(iStart == 512); //TODO this might not be very secure. What if user sends some data in smaller packages than 512? Or exactly 512 x times? That will break the program, recv blocks for ever.

        //Extracting domain name and path in user request. Test webpage input www.softwareqatest.com/qatfaq2.html
        int spaces_until_sep;
        int first_ln_len = 0;
        for(; initial_user_req[first_ln_len] != '\r'; first_ln_len++){ //Finding end of first line
            if(initial_user_req[first_ln_len] == '|') spaces_until_sep = first_ln_len; //Finding position of separator
        }
        int domain_length = stoi(initial_user_req.substr(0, spaces_until_sep));
        int path_length = stoi(initial_user_req.substr(spaces_until_sep + 1, first_ln_len - spaces_until_sep + 1));
        if(path_length == 0) path_length = -1;

        size_t hostPos = initial_user_req.find("Host: ");
        string domain_name = initial_user_req.substr(hostPos + 6, domain_length);
        initial_user_req = initial_user_req.substr(first_ln_len + 2, initial_user_req.length()); //Removing first line of request, no longer any need

        //getting IP address from domain sent in by user
        hostent *webDomain = gethostbyname(domain_name.c_str());
        in_addr *addr; //To get  char  version of ip: inet_ntoa(*addr)
        for(int i = 0; ; ++i) //Purposefully left the second condition out, because we will be testing for it inside the loop.
        {
            char *temp = webDomain->h_addr_list[i];
            if(temp == NULL) //we reached the end of the list
                break; //exit the loop.

            addr = reinterpret_cast<in_addr*>(temp);
        }
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            SOCKET web_page_socket = SocketGetters::getConnectSocket(inet_ntoa(*addr), "80"); //HTTP listens on port 80
            iSendResult = send(web_page_socket, initial_user_req.c_str(), initial_user_req.length(), 0); //forwarding received message to next/server
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

            //Redirecting response directly
            do {
                iResult = recv(web_page_socket, recvbuf, recvbuflen, 0); //Receiving from nextNode
                iSendResult = send(ClientSocket, recvbuf, recvbuflen, 0); //Sending back to prevNode immediately
                if (iSendResult == SOCKET_ERROR) {
                    printf("HAPPENED IN EXITNODE PLACE 2");
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
