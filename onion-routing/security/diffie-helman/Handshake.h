//
// Created by xray2 on 19/03/2022.
//

#ifndef IDATT2104___PROJECT___ONION_ROUTER_HANDSHAKE_H
#define IDATT2104___PROJECT___ONION_ROUTER_HANDSHAKE_H


#include "../../node/Node.h"
/**
 * Class for handshaking two nodes/keys
 */
class Handshake {
private:
public:
    static bool doHandshake(Node n1, Node n2);

    static BIGNUM *doHandshake(Key &k1, Key &k2);
};


#endif //IDATT2104___PROJECT___ONION_ROUTER_HANDSHAKE_H
