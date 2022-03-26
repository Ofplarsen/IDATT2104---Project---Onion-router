#include "onion-routing/node/Node.h"
#include "onion-routing/security/diffie-helman/SC.h"
#include "onion-routing/client/MainServer.h"


int main(int argc, char const *argv[]){
    MainServer m;
    m.start();
    return 0;
}