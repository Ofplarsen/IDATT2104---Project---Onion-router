#include <iostream>
#include "onion-routing/node/Node.h"
#include "onion-routing/security/diffie-helman/SC.h"
#include "onion-routing/security/aes/Crypter.h"
#include "onion-routing/security/model/Cryption.h"
#include "onion-routing/security/string-modifier/StringModifier.h"
#include "onion-routing/client/MainServer.h"
#include <vector>
#include <ctime>


#define P 1786534726771898
#define G 1234567890123
int main(int argc, char const *argv[]){

    MainServer main;

    vector<Node> nodes = main.getUserNodes();

    std::string text = "GET / HTTP/1.1\r\n"
                       "Host: localhost:1250\r\n"
                       "Connection: keep-alive\r\n"
                       "Cache-Control: max-age=0\r\n";

    //Cryption enc = Crypter::encrypt(text, BN_get_word(nodes[0].decryptKey.getSecretKey1()));
    string encrypted;
    string decrypted;
    vector<Cryption> c;
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

    encrypted = nodes[0].encrypt(text);
    nodes[0].
    c.push_back(nodes[0].encryptC(text));
    cout << std::hex << encrypted << endl;
    encrypted = nodes[1].encrypt(encrypted);
    c.push_back(nodes[1].encryptC(c[0]));
    cout <<std::hex << encrypted << endl;
    decrypted = nodes[2].decrypt(c[1]);
    cout << std::hex << decrypted << endl;
    decrypted = nodes[1].decrypt(c[0]);
    cout << std::hex <<decrypted << endl;
    return 0;
}