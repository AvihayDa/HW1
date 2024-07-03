#include "BlockChain.h"
#include "Utilities.h"
#include <sstream>
#include <iostream>

int BlockChainGetSize(const BlockChain& blockChain) {
        int size = 0;
        if (blockChain.time != "") {
                size++;
        }
        BlockChain* current = blockChain.nextBlock;
        while (current != nullptr) {
                size++; 
                current = current->nextBlock;
        }
        return size;
}

int BlockChainPersonalBalance(const BlockChain& blockChain, const string& name) {
        int balance = 0;
        if (blockChain.time != "") {
                if (blockChain.transaction.sender == name) {
                        balance -= blockChain.transaction.value;
                }
                if (blockChain.transaction.receiver == name) {
                        balance += blockChain.transaction.value;
                }
        }
        BlockChain* current = blockChain.nextBlock;
        while (current != nullptr) {
                if (current->transaction.sender == name) {
                        balance -= current->transaction.value;
                }
                if (current->transaction.receiver == name) {
                        balance += current->transaction.value;
                }
                current = current->nextBlock;
        }
        return balance;
}

void BlockChainAppendTransaction(
    BlockChain& blockChain,
    unsigned int value,
    const string& sender,
    const string& receiver,
    const string& timestamp
){
    if(blockChain.time == ""){
        blockChain.transaction.value = value;
        blockChain.transaction.sender = sender;
        blockChain.transaction.receiver = receiver;
        blockChain.time = timestamp;
        return;
    }
    BlockChain* newBlock = new BlockChain;
    newBlock->transaction.value = value;
    newBlock->transaction.sender = sender;
    newBlock->transaction.receiver = receiver;
    newBlock->time = timestamp;
    blockChain.nextBlock = newBlock;
}

void BlockChainAppendTransaction(
    BlockChain& blockChain,
    const Transaction& transaction,
    const string& timestamp
){
    if(blockChain.time == ""){
        blockChain.transaction = transaction;
        blockChain.time = timestamp;
        return;
    }
    BlockChain* newBlock = new BlockChain;
    newBlock->transaction = blockChain.transaction;
    newBlock->time = blockChain.time;
    blockChain.nextBlock = newBlock;
}

BlockChain BlockChainLoad(ifstream& file){
    BlockChain head;
    string line, sender, receiver, val, timestamp;
    unsigned int value = 0;
    BlockChain* blockChain = &head;
    while (file >> sender >> receiver >> val >> timestamp){
        value = stoi(val);
        BlockChainAppendTransaction(*blockChain, value, sender, receiver, timestamp);
        if(blockChain->nextBlock != nullptr){
            blockChain = blockChain->nextBlock;
        }
    }
    return head;
}

void BlockChainDump(const BlockChain& blockChain, ofstream& file){
    if (!file.is_open()){
        std::cout << "File not open" << std::endl;
        return;
    }
    if (blockChain.time == ""){
        std::cout << "BlockChain is empty" << std::endl;
        return;
    }
    const BlockChain* current = &blockChain;
    file << "BlockChain Info:" << std::endl;
    int i = 1;
    while(current != nullptr){
        file << i << "." << std::endl;
        file << "Sender Name: " << current->transaction.sender << std::endl;
        file << "Receiver Name: " << current->transaction.receiver << std::endl;
        file << "Transaction Value: " << current->transaction.value << std::endl;
        file << "Transaction timestamp: " << current->time << std::endl;
        current = current->nextBlock;
        i++;
    }
}

void BlockChainDumpHashed(const BlockChain& blockChain, ofstream& file){
    if (blockChain.time == ""){
        return;
    }
    const BlockChain* current = &blockChain;
    while(current != nullptr){
        // declare sender, receiver, value to avoid the code line of  "hash()" call to cross 100 characters
        string sender = current->transaction.sender;
        string receiver = current->transaction.receiver;
        unsigned int value = current->transaction.value;
        file << hash(value, sender, receiver);
        if (current->nextBlock != nullptr){
            file << std::endl;
        }
        current = current->nextBlock;
    }
}

bool BlockChainVerifyFile(const BlockChain& blockChain, std::ifstream& file){
    const BlockChain* current = &blockChain;
    string transactionID;
    while(current != nullptr){
        // declare sender, receiver, value to avoid the code line of  "hash()" call to cross 100 characters
        string sender = current->transaction.sender;
        string receiver = current->transaction.receiver;
        unsigned int value = current->transaction.value;
        file >> transactionID;
        if (transactionID != hash(value, sender, receiver)){
            return false;
        }
        current = current->nextBlock;
        if(file.eof() && current != nullptr){
            return false;
        }
    }
    if(!file.eof()){
        return false;
    }
    return true;
}

void BlockChainCompress(BlockChain& blockChain){
    int size = BlockChainGetSize(blockChain);
    if (blockChain.time == "" || size == 1){
        return;
    }
    BlockChain* current = &blockChain;
    BlockChain* next = current->nextBlock;
    while(next != nullptr){
        string currentSender = current->transaction.sender;
        string currentReceiver = current->transaction.receiver;
        string nextSender = next->transaction.sender;
        string nextReceiver = next->transaction.receiver;
        if (currentSender == nextSender && currentReceiver == nextReceiver){
            current->transaction.value += next->transaction.value;
            current->nextBlock = next->nextBlock;
            delete next;
            next = current->nextBlock;
        }
        else{
            current = next;
            next = next->nextBlock;
        }
    }
}


void BlockChainTransform(BlockChain& blockChain, updateFunction function){
    BlockChain* current = &blockChain;
    while(current != nullptr){
        current->transaction.value = function(current->transaction.value);
        current = current->nextBlock;
    }
}