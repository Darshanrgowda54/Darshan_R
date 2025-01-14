#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H
#include "user.h"


void loadAccountsFromFile();
void saveAccountToFile(user *newuser);
void deleteAccountInFile(int accountNumber);
void updateAccountInFile(int accountNumber, int fieldToUpdate, void *newValue);


void updateBalanceAfterTransaction(user *currentUser);
void saveTransactionToFile(user *currentUser, transaction *newTransaction);
void loadTransactionsFromFile();


#endif
