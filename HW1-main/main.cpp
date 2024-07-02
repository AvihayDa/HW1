#include <string>
#include <fstream>
#include <iostream>
#include "BlockChain.h"
#include "Utilities.h"
#include "Transaction.h"


void freeBlockChain(BlockChain& blockChain){
    BlockChain* current = blockChain.nextBlock;
    while(current != nullptr){
        BlockChain* next = current->nextBlock;
        delete current;
        current = next;
    }
}


int main(int argc, char *argv[]){
    if (argc != 4){
        std:: cout << getErrorMessage() << std::endl;
        return 0;
    }
    string command = argv[1];
    ifstream source(argv[2]);
    
    if (command == "format"){
        ofstream target(argv[3]);
        BlockChain blockChain = BlockChainLoad(source);
        BlockChainDump(blockChain, target);
        freeBlockChain(blockChain);
    }
    else if (command == "hash"){
        ofstream target(argv[3]);
        BlockChain blockChain = BlockChainLoad(source);
        BlockChainDumpHashed(blockChain, target);
        freeBlockChain(blockChain);
    }
    else if (command == "compress"){
        ofstream target(argv[3]);
        BlockChain blockChain = BlockChainLoad(source);
        BlockChainCompress(blockChain);
        BlockChainDump(blockChain, target);
        freeBlockChain(blockChain);
    }
    else if (command == "verify"){
        ifstream target(argv[3]);
        BlockChain blockChain = BlockChainLoad(source);
        if(BlockChainVerifyFile(blockChain, target)){
            std::cout << "Verification passed" << std::endl;
        }
        else{
            std::cout << "Verification failed" << std::endl;
        }
        freeBlockChain(blockChain);
    }
    else {
        std::cout << getErrorMessage() << std::endl;
    }
    return 0;
}