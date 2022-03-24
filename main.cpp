#include <iostream>
#include "onion-routing/node/Node.h"
#include "onion-routing/security/diffie-helman/SC.h"
#include "onion-routing/security/aes/Crypter.h"
#include "onion-routing/security/model/Cryption.h"
#include "onion-routing/security/string-modifier/StringModifier.h"
#include "onion-routing/client/MainServer.h"
#include "onion-routing/node/ExitNode.h"

#include <vector>
#include <ctime>
#include <thread>
#include <openssl/err.h>
#include <openssl/evp.h>

int main(int argc, char const *argv[]){

    MainServer main(3);
    vector<Node> nodes = main.getUserNodes();

    std::string text = "Lorem ipsum dolor sit amet, consectetuer iculus mus. Donec quamlis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui. Etiam rhoncus. Maecenas tempus, tellus eget condimentum rhoncus, sem quam semper libero, sit amet adipiscing sem neque sed ipsum. Nam quam nunc, blandit vel, luctus pulvinar, hendrerit id, lorem. Maecenas nec odio et ante tincidunt tempus. Donec vitae sapien ut libero venenatis faucibus. Nullam quis ante. Etiam sit amet orci eget eros faucibus tincidunt. Duis leo. Sed fringilla mauris sit amet nibh. Donec sodales sagittis magna. Sed consequat, leo eget bibendum sodales, augue velit cursus nunc, ";
    main.sendMessage(text);
    vector<string> s = StringModifier::splitString(text, 32);
    string d = nodes[0].encrypt(text);
    string f = nodes[1].encrypt(d);
    Cryption sd = Crypter::encrypt(text, main.keys[0]);
    Cryption sdd = Crypter::encrypt(text, main.keys[1]);
    cout << StringModifier::getLengthOfLastBlock(text) << endl;
    cout << StringModifier::getLengthOfLastBlock(d) << endl;
    cout << StringModifier::getLengthOfLastBlock(f) << endl;
    //    Cryption c, t;
//    c = Crypter::encrypt("Test strin to encrypt", main.keys[0]);
//    c = Crypter::encrypt(c, main.keys[1]);
//    c = Crypter::encrypt(c, main.keys[2]);
//    t  = Crypter::decrypt(c, main.keys[2]);
//    t  = Crypter::decrypt(t, main.keys[1]);
//    t  = Crypter::decrypt(t, main.keys[0]);
//    string s = StringModifier::cryptionToString(t);
//    cout << s << endl;
//    string encrypted;
//    string decrypted;
//    vector<Cryption> c;
//    encrypted = nodes[0].encrypt(text);
//    c.push_back(nodes[0].encryptC(text));
//    /*
//     * Working keys:
//     * 1340310318531409
//        1340310318531409
//        2483489254864132
//        2483489254864132
//        1069621193639077
//        1069621193639077
//
//        4140671936038427
//        4140671936038427
//        1623160691205860
//        1623160691205860
//        9332812852477698
//        9332812852477698
//
//        1011149782593260
//        1011149782593260
//        1120416084049417
//        1120416084049417
//        1064593576997070
//        1064593576997070
//        3955691958410533
//        3955691958410533
//        7767560767875582
//        7767560767875582
//        1720629364638329
//        1720629364638329
//        2023665742938981
//        2023665742938981
//        1382133548946841
//        1382133548946841
//        1746285431172213
//        1746285431172213
//     */
//    decrypted = nodes[1].decrypt(c[0]);
//    cout << std::hex <<decrypted << endl;
//
//    cout << "\n\n"<< endl;
//
//    encrypted = nodes[1].encrypt(encrypted);
//    c.push_back(nodes[1].encryptC(c[0]));
//
//    decrypted = nodes[2].decrypt(c[1]);
//    cout << std::hex << decrypted << endl;
//    decrypted = nodes[1].decrypt(c[0]);
//    cout << std::hex <<decrypted << endl;
//
//    cout << "\n\n"<< endl;
//
//
//
//    encrypted = nodes[2].encrypt(encrypted);
//    c.push_back(nodes[2].encryptC(c[1]));
//    cout <<std::hex << encrypted << endl;
//    decrypted = nodes[3].decrypt(c[2]);
//    cout << std::hex << decrypted << endl;
//    decrypted = nodes[2].decrypt(c[1]);
//    cout << std::hex << decrypted << endl;
//    decrypted = nodes[1].decrypt(c[0]);
//    cout << std::hex <<decrypted << endl;
//
//    c.pop_back();
//    c.pop_back();
//    c.pop_back();
//    /*
//    encrypted = nodes[0].encrypt(text);
//    c.push_back(nodes[0].encryptC(text));
//    encrypted = nodes[1].encrypt(encrypted);
//    c.push_back(nodes[1].encryptC(c[0]));
//    encrypted = nodes[2].encrypt(encrypted);
//    c.push_back(nodes[2].encryptC(c[1]));
//    cout <<std::hex << encrypted << endl;
//    decrypted = nodes[3].decrypt(c[2]);
//    cout << std::hex << decrypted << endl;
//    decrypted = nodes[2].decrypt(c[1]);
//    cout << std::hex << decrypted << endl;
//    decrypted = nodes[1].decrypt(c[0]);
//    cout << std::hex <<decrypted << endl;
//    */
//    c.push_back(nodes[0].encryptC(text));
//    for(int i = 1; i < 9; i++){
//        c.push_back(nodes[i].encryptC(c[i-1]));
//    }
//
//    for(int i = 9; i > 0; i--){
//        text = nodes[i].decrypt(c[i-1]);
//    }
//    cout << text << endl;
    /*
    c.pop_back();
    c.pop_back();
    c.pop_back();
    encrypted = nodes[0].encrypt(text);
    c.push_back(nodes[0].encryptC(text));
//    encrypted = nodes[1].encrypt(encrypted);
//    c.push_back(nodes[1].encryptC(c[0]));
//    encrypted = nodes[2].encrypt(encrypted);
//    c.push_back(nodes[2].encryptC(c[1]));
//    cout <<std::hex << encrypted << endl;
//    decrypted = nodes[3].decrypt(c[2]);
//    cout << std::hex << decrypted << endl;
//    decrypted = nodes[2].decrypt(c[1]);
//    cout << std::hex << decrypted << endl;
    decrypted = nodes[1].decrypt(c[0]);
    cout << std::hex <<decrypted << endl;
*/
  //  cout << text.compare(decrypted) << endl;
//    char recvbuf[512];
//    int recvbuflen = 512;
//    string test = "";
//    SOCKET receive = nodes[0].getListenSocket("8081");
//    //SOCKET ClientSocket = accept(receive, NULL, NULL);
//    if (ClientSocket == INVALID_SOCKET) {
//        printf("accept failed: %d\n", WSAGetLastError());
//        closesocket(receive);
//        WSACleanup();
//        return -1;
//    }
//    SOCKET sender = nodes[0].getConnectSocket("192.168.10.100", "8081");
//    int res = send(sender,encrypted.c_str(), encrypted.length(), 0);
//    cout << encrypted.c_str() << endl;
//    cout << encrypted << endl;
//    int res2 =  recv(receive, recvbuf, recvbuflen, 0);
//    if (res2 == SOCKET_ERROR) {
//        printf("send failed: %d\n", WSAGetLastError());
//        closesocket(receive);
//        closesocket(sender);
//        WSACleanup();
//        return 0;
//    }
//    test += string(recvbuf).substr(0, res2); //Gathering user request in a string
//    cout << test << endl << endl;
//    closesocket(receive);
//    closesocket(sender);
//    WSACleanup();
//    Node n1;
//    InputNode inp1;
//    ExitNode exn1;
//    n1.nextNode = &exn1;
    //inp1.initialize_server_socket(8081);
//std::thread t1(&InputNode::initialize_server_socket, &inp1, "8081");
//    std::thread t2(&Node::initialize_server_socket, &inp1, "8087", "8080");
//    std::thread t3(&ExitNode::initialize_server_socket, &exn1, "8080");
//    //n1.initialize_server_socket("8080");
//
//    t1.join();
//    t2.join();
//    t3.join();
    return 0;

    //Cryption enc = Crypter::encrypt(text, BN_get_word(nodes[0].decryptKey.getSecretKey1()));
//    string encrypted;
//    string decrypted;
//    vector<Cryption> c;
//    for(int i = 0; i < 3; i++){
//
//        encrypted = nodes[i].encrypt(text);
//        c.push_back(nodes[i].encryptC(text));
//        text = encrypted;
//
//    }
//    cout << encrypted << endl;
//    int x = 2;
//    for (int i = 3; i > 0; --i) {
//        decrypted = nodes[i].decrypt(c[x]);
//        x--;
//    }
//    cout << decrypted << endl;

//    encrypted = nodes[0].encrypt(text);
//    c.push_back(nodes[0].encryptC(text));
//    encrypted = nodes[1].encrypt(encrypted);
//    c.push_back(nodes[1].encryptC(c[0]));
//    cout <<std::hex << encrypted << endl;
//    decrypted = nodes[2].decrypt(c[1]);
//    cout << std::hex << decrypted << endl;
//    decrypted = nodes[1].decrypt(c[0]);
//    cout << std::hex <<decrypted << endl;
    return 0;
}