#include "Transaction.h"
#include "Utilities.h"

void TransactionDumpInfo(const Transaction& transaction, ofstream& file){
    file << "Sender Name: " << transaction.sender << std::endl;
    file << "Receiver Name: " << transaction.receiver << std::endl;
    file << "Transaction Value: " << transaction.value << std::endl;
}

string TransactionHashedMessage(const Transaction& transaction){
    return hash(transaction.value, transaction.sender, transaction.receiver);
}

bool TransactionVerifyHashedMessage(const Transaction& transaction, string hashedMessage){
    return TransactionHashedMessage(transaction) == hashedMessage;
}