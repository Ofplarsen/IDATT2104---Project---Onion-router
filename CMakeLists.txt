cmake_minimum_required(VERSION 3.20)
project(IDATT2104___Project___Onion_router)

set(CMAKE_CXX_STANDARD 14)


add_executable(IDATT2104___Project___Onion_router main.cpp onion-routing/node/Node.cpp onion-routing/node/Node.h onion-routing/key/Key.cpp onion-routing/key/Key.h onion-routing/client/Client.cpp onion-routing/client/Client.h onion-routing/node/InputNode.cpp onion-routing/node/InputNode.h onion-routing/node/ExitNode.cpp onion-routing/node/ExitNode.h onion-routing/diffie-helman/SC.cpp onion-routing/diffie-helman/SC.h onion-routing/diffie-helman/Crypter.cpp onion-routing/diffie-helman/Crypter.h)


target_link_libraries(IDATT2104___Project___Onion_router ../Libs/cryptopp/aes.h)

if(MINGW)
    target_link_libraries(IDATT2104___Project___Onion_router ws2_32)
endif()