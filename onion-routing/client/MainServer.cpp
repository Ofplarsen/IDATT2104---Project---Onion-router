//
// Created by xray2 on 14/03/2022.
//

#include "MainServer.h"
#include "../socket/SocketGetters.h"
#include <limits>

void generateKeys(){

}

int MainServer::start() {
    //create nodePool with x Nodes and give them their
    //ask for nodeAmount, at least 3
    nodeAmount = /*getNodeAmount(3, 12);*/ 3;
    //pick nodeAmount Nodes from pool
    //inform user about what to do next (log on with browser)
    cout << "Please go to your preferred browser and enter 'localhost:777' to begin using the program." << endl;
    //initiate server socket and forward socket
    SOCKET mainServer = SocketGetters::getListenSocket("777"); //TODO maybe randomly generate port_nr for security reasons?
    SOCKET clientConnection = INVALID_SOCKET;
    clientConnection = accept(mainServer, NULL, NULL); //Trying to accept a client connection
    if (clientConnection == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(mainServer);
        WSACleanup();
        return -1;
    }

#define DEFAULT_BUFLEN 512

    char recvbuf[DEFAULT_BUFLEN];
    int iResult, iSendResult;
    int recvbuflen = DEFAULT_BUFLEN;
    string userRequest;
    string userCommand;
    //size_t end;

    // Receive until the peer shuts down the connection
    do{

        iResult = recv(clientConnection, recvbuf, recvbuflen, 0);
        if(iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            userRequest+=string(recvbuf).substr(0, iResult);
            //end = userRequest.find("\r\n\r\n");
            userCommand = parseGetReq(userRequest);
            cout<<userCommand<<endl;
            if(userCommand == "INVALID") {
                cout << "An invalid request was sent to the server" << endl;
                break;

            }
            //when user logged on, send welcome message. 'type localhost:x/help for user manual' TODO need parser
            if(userCommand ==  ""){
                string welcomeMsg = welcome();
                send(clientConnection, welcomeMsg.c_str(), welcomeMsg.length(), 0);
            }
            if(userCommand == "help"){
                string helpMsg = help();
                send(clientConnection, helpMsg.c_str(), helpMsg.length(), 0);
            }



            /*  // Echo the buffer back to the sender
              iSendResult = send(clientConnection, recvbuf, iResult, 0);
              if (iSendResult == SOCKET_ERROR) {
                  printf("send failed: %d\n", WSAGetLastError());
                  closesocket(clientConnection);
                  WSACleanup();
                  return 1;
              }
              printf("Bytes sent: %d\n", iSendResult);*/
        }
        else if(iResult == 0){
            cout << "Connection closing..." << endl;
            userRequest.clear(); //Clearing user request so as not to get invalid string
            iResult = shutdown(clientConnection, SD_SEND); //No more sending info to clientConnection
            if(iResult == SOCKET_ERROR){
                printf("shutdown failed: %d\n", WSAGetLastError());
                closesocket(clientConnection);
                WSACleanup();
                return -2;
            }
            closesocket(clientConnection); //Closing the clientConnection

            cout << "Reestablishing connection..." << endl; //Reestablishing
            clientConnection = accept(mainServer, NULL, NULL); //Trying to accept a client connection
            if(clientConnection == INVALID_SOCKET){
                printf("accept failed: %d\n", WSAGetLastError());
                closesocket(mainServer);
                WSACleanup();
                return -1;
            }
        }
        else{
            printf("recv failed: %d\n", WSAGetLastError());
            closesocket(clientConnection);
            WSACleanup();
            return -3;
        }
    } while(userCommand != "exit");

    cout << userCommand << iResult << endl;
    //if user sends in something else than preprogrammed cases, send to InputNode from userNodes vector and hope for the best
    //enter 'localhost:x/exit' to terminate program

    // shutdown the send half of the connection since no more data will be sent
    iResult = shutdown(clientConnection, SD_SEND);
    if(iResult == SOCKET_ERROR){
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(clientConnection);
        WSACleanup();
        return 1;
    }
    // cleanup
    closesocket(clientConnection);
    WSACleanup();

    return 0;
}

string MainServer::parseGetReq(string req){ //TODO make req const -Clang(?)
    string firstLineEnd = " HTTP/1.1\r\n";
    int firstLineEndPos = req.find(firstLineEnd);
    if(firstLineEndPos == -1) return "INVALID";
    string command = req.substr(5, firstLineEndPos - 5); //Always 5 positions until path starts.
    return command;
}

int MainServer::getNodeAmount(int min, int max){
    int nodeAmountPlaceholder;
    cout << "Enter how many nodes you want to use, minimum " << min << " and maximum " << max << endl;
    while(!(cin >> nodeAmountPlaceholder) || nodeAmountPlaceholder < min || nodeAmountPlaceholder > max ){ //Need an invalid input
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "You entered an invalid number, pick an int between 3 and 12: " << endl;
    }
    return nodeAmountPlaceholder;
}

string MainServer::welcome(){
    string body =
            "<h1>Welcome to the Shrouter</h1>\r\n"
            "<p>This is a prototype implementation of an onion router</p>\r\n"
            "<p>To navigate to the help page, enter '<a href=\"localhost:777/help\">localhost:777/help</a>' in the address bar</p>"; //TODO refactor the portnr if it changes
    string welcome = "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html\r\n"
                     "Content-Length: " + to_string(body.length()) + "\r\n"
                     "Connection: Close\r\n\r\n"
                     + body;
    return welcome;
}

string MainServer::help(){
    string body =
            "<h1>Help</h1>\r\n"
            "This is a list with some rules regarding how to use the application"
            "<ul>"
            "<li>Commands should always be formatted as 'localhost:portnr/command'</li>"
            "<li>URLs should always be formatted as 'localhost:portnr/www.domain-name.extension/path-if-any'</li>"
            "<li>The current version only supports HTTP</li>"
            "<li>If you cannot normally navigate to an HTTP site via the IP address you get for it running the command nslookup, it is not supported (e.g. 1.1.1.1/80)</li>"
            //"<li></li>"
            "</ul>";
    string help = "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html\r\n"
                     "Content-Length: " + to_string(body.length()) + "\r\n"
                     "Connection: Close\r\n\r\n"
                     + body;
    return help;
}