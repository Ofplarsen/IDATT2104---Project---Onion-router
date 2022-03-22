//
// Created by xray2 on 14/03/2022.
//

#include "MainServer.h"
#include "../socket/SocketGetters.h"
#include <limits>
#include <thread>
#include <regex>

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
    int iResult, iSendResult;
    int recvbuflen = DEFAULT_BUFLEN;
    string userCommand;
    string userRequest;


    // Receive until the peer shuts down the connection
    char recvbuf[DEFAULT_BUFLEN];
    const regex pattern("(www.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");
    do{
        size_t end;
        iResult = recv(clientConnection, recvbuf, recvbuflen, 0);
        if(iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            userRequest+=string(recvbuf).substr(0, iResult);
            cout<<userRequest<<endl;
            end = userRequest.find("\r\n\r\n");
            if(end == string::npos) continue; //If we don't have the entire get request, continue to next iteration and get more of it
            userCommand = parseGetReq(userRequest);
            cout<<userCommand<<endl;
            if(userCommand == "INVALID") {
                cout << "An invalid request was sent to the server" << endl;
                break;
            }
            //when user logged on, send welcome message
            if(userCommand.empty()){
                string welcomeMsg = welcome();
                send(clientConnection, welcomeMsg.c_str(), welcomeMsg.length(), 0);
            }
            if(userCommand == "favicon.ico"){
                string notFoundMsg = notFound();
                send(clientConnection, notFoundMsg.c_str(), notFoundMsg.length(), 0);
            }
            //type localhost:x/help for help
            else if(userCommand == "help"){
                string helpMsg = help();
                send(clientConnection, helpMsg.c_str(), helpMsg.length(), 0);
            }
            //if user sends in something else than preprogrammed cases, send to InputNode from userNodes vector and hope for the best
            else if(regex_match(userCommand, pattern)){
                Node n1;
                InputNode inp1;
                ExitNode exn1;
                std::thread t1(&InputNode::initialize_server_socket, &inp1, "8081");
                std::thread t2(&Node::initialize_server_socket, &inp1, "8087", "8080");
                std::thread t3(&ExitNode::initialize_server_socket, &exn1, "8080");
                SOCKET connectSocket = SocketGetters::getConnectSocket("192.168.1.14", "8081");
                iSendResult = send(connectSocket, userRequest.c_str(), userRequest.length(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("HAPPENED IN MAINSERVER PLACE 1");
                    printf("send failed: %d\n", WSAGetLastError());
                    closesocket(clientConnection);
                    WSACleanup();
                    return 1;
                }
                printf("Bytes sent: %d\n", iSendResult);

                string response; //Gathering all response information in a string
                do {
                    iResult = recv(connectSocket, recvbuf, recvbuflen, 0); //Receiving from nextNode
                    //string recBufStr(recvbuf);
                    //cout<<recBufStr<<endl;
                    //if(recBufStr.find("</html>") != string::npos) break; //Found end of html, breaking loop
                    if (iResult > 0) {
                        printf("Bytes received: %d", iResult);
                        response += string(recvbuf).substr(0, iResult);
                    }
                    else if (iResult == 0)
                        printf("Connection with next closed\n");
                    else
                        printf("recv failed: %d\n", WSAGetLastError());
                } while (iResult > 0);

                //Extracting content length from header
                size_t contLenPos = response.find("Content-Length: "); //Find position of string
                size_t newLnAfterContLen = response.find("\r\n", contLenPos); //Find first newline after contLenPos
                int contLen = stoi(response.substr(contLenPos+16, newLnAfterContLen));

                //Finding how long the headers are
                size_t headerEndPos = response.find("\r\n\r\n");
                //Getting only the part of the response we need
                response = response.substr(0, contLen + headerEndPos + 4); //TODO not sure about the offset
                //cout <<response<< endl;
                cout <<response<< endl;
                cout << "Header end "<<headerEndPos << endl;
                cout << "Cont len "<<contLen << endl;
                //cout << recvbuf << endl;
                printf("final %d\n", iSendResult);

                t1.join();
                t2.join();
                t3.join();
                iSendResult = send(clientConnection, response.c_str(), response.length(), 0); //Sending back to prevNode immediately

                if (iSendResult == SOCKET_ERROR) {
                    printf("HAPPENED IN MAINSERVER PLACE 2");
                    printf("send failed: %d\n", WSAGetLastError());
                    cout << recvbuf << endl;
                    closesocket(clientConnection);
                    WSACleanup();
                    t1.join();
                    t2.join();
                    t3.join();
                    return -4;
                }
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
        else if(iResult == 0){ //Client has closed the connection, need to do the same and reestablish to continue loop
            cout << "Connection closing..." << endl;
            //userRequest.clear(); //Clearing user request so as not to get invalid string
            string().swap(userRequest); //Removing space allocated for uerRequest
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
    } while(userCommand != "exit"); //enter 'localhost:x/exit' to terminate program


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
            "<p>To navigate to the help page, enter '<a href=\"http://localhost:777/help\">localhost:777/help</a>' in the address bar</p>"; //TODO refactor the portnr if it changes
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
            "<li>Only works with Windows</li>"
            //"<li></li>"
            "</ul>";
    string help = "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html\r\n"
                     "Content-Length: " + to_string(body.length()) + "\r\n"
                     "Connection: Close\r\n\r\n"
                     + body;
    return help;
}

string MainServer::notFound(){
    string notFound = "HTTP/1.0 404 Not Found\r\n"
                      "Connection: Close\r\n\r\n";
    return notFound;
}