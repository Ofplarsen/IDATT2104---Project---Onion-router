//
// Created by xray2 on 14/03/2022.
//

#include "MainServer.h"
#include <limits>
#include <thread>
#include <regex>
#include "../security/aes/Crypter.h"

//Websites for testing: www.example.com, www.softwareqatest.com, www.columbia.edu/~fdc/sample.html (something goes wrong here), www.virtu-software.com (something goes wrong here)
/**
 * Starts the application letting the user connect to a socket on port 777
 *
 * @return 0 if exited cleanly, 1 if not
 */
int MainServer::start() {
    //Getting localhosts IP from user
    const char* localhost = getLocalhostIpFromUser().c_str(); //"192.168.56.1";
    //inform user about what to do next (log on with browser)
    cout << "Please go to your preferred browser and enter 'localhost:777' to begin using the program." << endl;
    //initiate server socket and forward socket
    SOCKET mainServer = SocketGetters::getListenSocket("777");
    SOCKET clientConnection = INVALID_SOCKET;
    clientConnection = accept(mainServer, NULL, NULL); //Trying to accept a client connection
    if (clientConnection == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(mainServer);
        WSACleanup();
        return 1;
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
            end = userRequest.find("\r\n\r\n");
            if(end == string::npos) continue; //If we don't have the entire get request, continue to next iteration and get more of it
            userCommand = parseGetReq(userRequest);
            cout<<"User sent in "<<userCommand<<endl;
            if(userCommand == "exit") continue; //TODO maybe exit cleaner
            if(userCommand == "INVALID") {
                cout << "An invalid request was sent to the server" << endl;
                break;
            }
            //when user logged on, send welcome message
            else if(userCommand.empty()){
                string welcomeMsg = welcome();
                send(clientConnection, welcomeMsg.c_str(), welcomeMsg.length(), 0);
            }
            else if(userCommand == "favicon.ico"){
                string notFoundMsg = notFound(userCommand);
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
                std::thread t1(&InputNode::initialize_server_socket, &inp1, "8081", "8087", localhost);
                std::thread t2(&Node::initialize_server_socket, &n1, "8087", "8080", localhost);
                std::thread t3(&ExitNode::initialize_server_socket, &exn1, "8080");

                SOCKET connectSocket = SocketGetters::getConnectSocket(localhost, "8081");
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
                int contLen = stoi(response.substr(contLenPos+16, newLnAfterContLen)); //Content length

                //Finding how long the headers are
                size_t headerEndPos = response.find("\r\n\r\n");
                //Getting only the part of the response we need
                response = response.substr(0, contLen + headerEndPos + 4); //Need offset to account for newlines

                printf("final %d\n", iSendResult);

                //Threads are done
                t1.join();
                t2.join();
                t3.join();

                //Images not yet supported, if headers include image/jpeg, send 404 to user
                if(response.find("Content-Type: image/jpeg") != string::npos) response = MainServer::notFound("image");

                //Sending back to client
                iSendResult = send(clientConnection, response.c_str(), response.length(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("HAPPENED IN MAINSERVER PLACE 2");
                    printf("send failed: %d\n", WSAGetLastError());
                    closesocket(clientConnection);
                    WSACleanup();
                    return 1;
                }
            }
            //If command is not found
            else{
                string notFoundMsg = notFound(userCommand);
                send(clientConnection, notFoundMsg.c_str(), notFoundMsg.length(), 0);
            }
        }
        else if(iResult == 0){ //Client has closed the connection, need to do the same and reestablish to continue loop
            cout << "Connection closing..." << endl;
            string().swap(userRequest); //Removing space allocated for uerRequest
            iResult = shutdown(clientConnection, SD_SEND); //No more sending info to clientConnection
            if(iResult == SOCKET_ERROR){
                printf("shutdown failed: %d\n", WSAGetLastError());
                closesocket(clientConnection);
                WSACleanup();
                return 1;
            }
            closesocket(clientConnection); //Closing the clientConnection

            cout << "Reestablishing connection..." << endl; //Reestablishing
            clientConnection = accept(mainServer, NULL, NULL); //Trying to accept a client connection
            if(clientConnection == INVALID_SOCKET){
                printf("accept failed: %d\n", WSAGetLastError());
                closesocket(mainServer);
                WSACleanup();
                return 1;
            }
            cout<<"Ready for new command/URL"<<endl;
        }
        else{
            printf("recv failed: in mainserver %d\n", WSAGetLastError());
            closesocket(clientConnection);
            WSACleanup();
            return 1;
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

/**
 * Parses get request from browser to find command sent in by user
 * @param req request from browser
 * @return a string containing the user command
 */
string MainServer::parseGetReq(string req){ //TODO make req const -Clang(?)
    string firstLineEnd = " HTTP/1.1\r\n";
    int firstLineEndPos = req.find(firstLineEnd);
    if(firstLineEndPos == -1) return "INVALID";
    string command = req.substr(5, firstLineEndPos - 5); //Always 5 positions until path starts.
    return command;
}

/**
 * deprecated
 * Lets user choose a number in a range for how many Nodes they want to use.
 *
 * @param min minimum amount of Nodes
 * @param max maximum amount of Nodes
 * @return amount of Nodes the user wants
 */
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

/**
 * HTTP response greeting the user.
 *
 * @return a string containing the HTTP greeting
 */
string MainServer::welcome(){
    string body =
            "<h1>Welcome to the Simple router</h1>\r\n"
            "<p>This is a prototype implementation of an onion router</p>\r\n"
            "<p>To navigate to the help page, enter '<a href=\"http://localhost:777/help\">localhost:777/help</a>' in the address bar</p>"; //TODO refactor the portnr if it changes
    string welcome = "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html\r\n"
                     "Content-Length: " + to_string(body.length()) + "\r\n"
                     "Connection: Close\r\n\r\n"
                     + body;
    return welcome;
}

/**
 * HTTP used to give simple info to user.
 * @return
 */
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
            "<li>Disable cache for the most reliable, but slow, experience</li>"
            "<li>Not compatible with images</li>"
            //"<li></li>"
            "</ul>";
    string help = "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html\r\n"
                     "Content-Length: " + to_string(body.length()) + "\r\n"
                     "Connection: Close\r\n\r\n"
                     + body;
    return help;
}

/**
 * HTTP used if a command is not found.
 * @param command the command that is not found
 * @return
 */
string MainServer::notFound(string command){
    string body =
            "<h1>404 Not Found</h1>"
            "<p>No support for command '" + command + "'</p>";
    string notFound = "HTTP/1.1 404 Not Found\r\n"
                      "Content-Type: text/html\r\n"
                      "Content-Length: " + to_string(body.length()) + "\r\n"
                      "Connection: Close\r\n\r\n"
                      +body;
    return notFound;
}

/**
 * Gets localhost IP from the user
 * @return
 */
string MainServer::getLocalhostIpFromUser(){
    string localhostIP;
    cout << "Please enter the IP address of your localhost: ";
    cin >> localhostIP;
    return localhostIP;
}
