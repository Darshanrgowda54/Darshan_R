#ifndef USER_H
#define USER_H
#include "bankaccount.h"
#include "transaction.h"

typedef struct user
{
    char userID[10];
    char userPassword[10];
    char name[25];
    char mobile[12];
    char email[25];
    char address[30];
    char status;
    bankAccount account;
    transaction *transactions;
    struct user *next;
} user;

extern user *head;

void userLogin();
void userMenu(user *loggedInUser);
void deposit(user *loggedInUser);
void withdraw(user *loggedInUser);
void transfer(user *loggedInUser);
void checkBalance(user *loggedInUser);
void viewTransactionHistory(user *loggedInUser);


#endif
