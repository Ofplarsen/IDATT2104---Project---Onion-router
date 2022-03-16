#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>
#include "onion-routing/node/Node.h"
#include "onion-routing/node/ExitNode.h"
#include "onion-routing/diffie-helman/SC.h"
#include "onion-routing/diffie-helman/Crypter.h"
#define PORT 8080

int main(int argc, char const *argv[])
{
    Key annaKey;
    Key bobKey;
    long long int publicKey = SC::generatePublicKey();
    long long int primitiveRoot = SC::findPrimitiveRoot(publicKey);
    annaKey.setPublicKeyP(publicKey);
    annaKey.setPublicKeyG(primitiveRoot);
    bobKey.setPublicKeyP(publicKey);
    bobKey.setPublicKeyG(primitiveRoot);

    cout << publicKey << endl;
    cout << primitiveRoot << endl;

    annaKey.setPrivateKey(13);
    bobKey.setPrivateKey(17);

    cout << annaKey.getSecretKey(bobKey.generateKey()) << endl;
    cout << bobKey.getSecretKey(annaKey.generateKey()) << endl;
    int encrypted = Crypter::encrypt('d', annaKey.getSecretKey(bobKey.generateKey()), annaKey.getPublicKeyP());
    cout << encrypted << endl;
    cout << static_cast<char>(Crypter::decrypt(encrypted, annaKey.getSecretKey(bobKey.generateKey()), annaKey.getPublicKeyP())) << endl;
    return 0;
}