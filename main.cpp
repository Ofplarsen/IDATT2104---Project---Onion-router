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

    MainServer mains;
    mains.start();
    return 0;
}