cmake_minimum_required(VERSION 3.16)
project(HW1)

set(CMAKE_CXX_STANDARD 17)

add_library(mtm_blockchain_lib
        HW1-main/Utilities.cpp
        HW1-main/BlockChain.cpp
        HW1-main/Transaction.cpp

        HW1-main/BlockChain.h
        HW1-main/Transaction.h
        HW1-main/Utilities.h
)

target_include_directories(
        mtm_blockchain_lib
        PUBLIC
        .
)

add_executable(
        mtm_blockchain

        HW1-main/main.cpp
)

target_link_libraries(mtm_blockchain mtm_blockchain_lib)


install(TARGETS mtm_blockchain DESTINATION bin COMPONENT mtm_blockchain)
