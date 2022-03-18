cmake_minimum_required(VERSION 3.20)
project(IDATT2104___Project___Onion_router)

set(CMAKE_CXX_STANDARD 14)


add_executable(IDATT2104___Project___Onion_router main.cpp onion-routing/node/Node.cpp onion-routing/node/Node.h onion-routing/security/key/Key.cpp onion-routing/security/key/Key.h onion-routing/client/Client.cpp onion-routing/client/Client.h onion-routing/node/InputNode.cpp onion-routing/node/InputNode.h onion-routing/node/ExitNode.cpp onion-routing/node/ExitNode.h onion-routing/security/diffie-helman/SC.cpp onion-routing/security/diffie-helman/SC.h onion-routing/security/aes/Crypter.cpp onion-routing/security/aes/Crypter.h onion-routing/security/string-modifier/StringModifier.cpp onion-routing/security/string-modifier/StringModifier.h onion-routing/security/model/Cryption.cpp onion-routing/security/model/Cryption.h)
find_package(OpenSSL REQUIRED)
target_link_libraries(IDATT2104___Project___Onion_router OpenSSL::SSL)

if(MINGW)
    target_link_libraries(IDATT2104___Project___Onion_router ws2_32)
endif()