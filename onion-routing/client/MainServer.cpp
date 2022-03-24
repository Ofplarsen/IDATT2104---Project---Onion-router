//
// Created by xray2 on 14/03/2022.
//

#include <iostream>
#include "MainServer.h"
#include "../socket/SocketGetters.h"
#include <limits>
#include <thread>
#include <regex>
#include "../security/diffie-helman/Handshake.h"
#include "../security/string-modifier/StringModifier.h"
#include "../security/aes/Crypter.h"

#define localhost "192.168.10.100"
bool MainServer::generateKeys(){
    for(auto it = begin(userNodes); it != end(userNodes)-1; it++){
        long long int num = StringModifier::BN2LLI(Handshake::doHandshake(it->decryptKey, std::next(it)->encryptKey));
        keys.push_back(num);
    }

}

Cryption MainServer::encrypt(string text){
    vector<Cryption> c;
    c.push_back(Crypter::encrypt(text, keys[0]));
    for(int i = 1; i < userNodes.size()-1; i++){
        c.push_back(Crypter::encrypt(c[i-1], keys[i]));
    }

    return c[userNodes.size()-2];
}

string MainServer::decrypt(Cryption &c) {
    Cryption text;
    vector<Cryption> cr;
    vector<string> s;
    cr.push_back(c);
    int x = 0;
    for(int i = userNodes.size()-2; i >= 0; i--){
        text = Crypter::decrypt(cr[x], keys[i]);
        cr.push_back(text);
        x++;
    }
    return StringModifier::cryptionToString(cr[userNodes.size()-1]);
}


const vector<Node> &MainServer::getUserNodes() const {
    return userNodes;
}

string MainServer::sendMessage(string message){
    Cryption encrypted = encrypt(message);

    string encryptedString = encrypted.getRequestString() += StringModifier::cryptionToString(encrypted);

    //receiveMessage(encrypted);
    return encryptedString;
}

string MainServer::receiveMessage(Cryption &c){
    string t = decrypt(c);
    return t;
}

//Websites for testing: www.example.com, www.softwareqatest.com, www.columbia.edu/~fdc/sample.html (something goes wrong here), www.virtu-software.com (something goes wrong here)

int MainServer::start() {
    //create nodePool with x Nodes and give them their
    nodeAmount = getNodeAmount(3, 12);
    initNodes();
    //ask for nodeAmount, at least 3
    generateKeys();
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
            end = userRequest.find("\r\n\r\n");
            if(end == string::npos) continue; //If we don't have the entire get request, continue to next iteration and get more of it
            userCommand = parseGetReq(userRequest);
            //cout<<userRequest<<endl;
            cout<<userCommand<<endl;
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
                for(int i = 0; i < userNodes.size(); i++){
                    userNodes[i].printError();
                }
//                std::thread t1(&InputNode::receiveAndSend, &userInputNode);
//                std::thread t2(&Node::receiveAndSend, &userNodes[1]);
//                std::thread t3(&ExitNode::receiveAndSend, &userExitNode);
                Node n1;
                InputNode inp1;
                ExitNode exn1;
                std::thread t1(&InputNode::initialize_server_socket, &userInputNode, "8081", "8087", localhost);
                std::thread t2(&Node::initialize_server_socket, &userNodes[1], "8087", "8080", localhost);
                std::thread t3(&ExitNode::initialize_server_socket, &userExitNode, "8080");

                SOCKET connectSocket = SocketGetters::getConnectSocket("192.168.10.100", "8081");

                userRequest = sendMessage(userRequest);

                vector<string> parsed = InputNode::parse_initial_request(userRequest); //Contains domain_name and path

                //constructing a get request that the node will send to socket on internet
                string getReq = InputNode::construct_get_request(parsed.at(0), parsed.at(1));

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
                size_t end;
                string info;

                end = response.find("\r\n");
                if(end != string::npos){
                    info = response.substr(0, end);
                    response = response.substr(end+2, response.length());
                }
                vector<int> sizes = split(info);
                response= response.substr(0, sizes[0]*sizes[1]+sizes[2]); //TODO MEM?


                Cryption cre = StringModifier::splitString(response);

                cre.strings_len = StringModifier::getVector(sizes[0],sizes[1],sizes[2]);

                response = receiveMessage(cre);

                //Extracting content length from header
                size_t contLenPos = response.find("Content-Length: "); //Find position of string
                size_t newLnAfterContLen = response.find("\r\n", contLenPos); //Find first newline after contLenPos
                int contLen = stoi(response.substr(contLenPos+16, newLnAfterContLen)); //Content length

                //Finding how long the headers are
                size_t headerEndPos = response.find("\r\n\r\n");
                //Getting only the part of the response we need
                response = response.substr(0, contLen + headerEndPos + 4); //TODO not sure why the offset works, but it does
                //cout <<response<< endl;
                cout <<response<< endl;
                cout << "Header end "<<headerEndPos << endl;
                cout << "Cont len "<<contLen << endl;
                //cout << recvbuf << endl;
                printf("final %d\n", iSendResult);

                //Threads are done
                t1.join();
                t2.join();
                t3.join();

                //Sending back to client
                iSendResult = send(clientConnection, response.c_str(), response.length(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("HAPPENED IN MAINSERVER PLACE 2");
                    printf("send failed: %d\n", WSAGetLastError());
                    cout << recvbuf << endl;
                    closesocket(clientConnection);
                    WSACleanup();
                    return -4;
                }
            }
            //If command is not found
            else{
                string notFoundMsg = notFound();
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

string MainServer::notFound(){
    string notFound = "HTTP/1.0 404 Not Found\r\n"
                      "Connection: Close\r\n\r\n";
    return notFound;
}

void MainServer::initNodes(){
    userNodes.push_back(InputNode("8081", "8087", "192.168.10.100"));

    for(int i = 0; i < nodeAmount-2; i++){
        userNodes.push_back(Node("8087", "8080", "192.168.10.100"));
    }
    ExitNode exn1("8080", "0" ,"0");
    userNodes.push_back(exn1);
}

vector<int> MainServer::split(string s){
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
    return info;
}

