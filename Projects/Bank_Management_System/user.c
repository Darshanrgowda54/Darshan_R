#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "fileoperations.h"


void userLogin() {
    char userID[10], userPassword[10];
    user *temp = head;

    printf("Enter User ID: ");
    scanf("%s", userID);
    printf("Enter User Password: ");
    scanf("%s", userPassword);

    while (temp != NULL) {
        if (strcmp(temp->userID, userID) == 0 && strcmp(temp->userPassword, userPassword) == 0)
        {
            if (temp->status == 'A')
            {
                printf("Account login successful\n");
                printf("Account Name :%s\n",temp->name);
                userMenu(temp);
                return;
            }
            else
            {
                printf("This account is inactive. Please contact the admin.\n");
                return;
            }
        }
        temp = temp->next;
    }
    printf("Invalid user credentials.\n");
}



enum userMenuOption
{
    DEPOSIT = 1,
    WITHDRAW,
    TRANSFER,
    CHECKBALANCE,
    VIEWTRANSACTIONHISTORY,
    EXIT
};


void userMenu(user *loggedInUser)
{
    int choice;

    while (1)
    {
        printf("\n________ User Menu ________\n");
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Transfer\n");
        printf("4. Check Balance\n");
        printf("5. View Transaction History\n");
        printf("6. Logout from User\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
        case DEPOSIT:
            deposit(loggedInUser);
            break;

        case WITHDRAW:
            withdraw(loggedInUser);
            break;

        case TRANSFER:
            transfer(loggedInUser);
            break;

        case CHECKBALANCE:
            checkBalance(loggedInUser);
            break;

        case VIEWTRANSACTIONHISTORY:
            viewTransactionHistory(loggedInUser);
            break;

        case EXIT:
            printf("________ Logout form User ________\n");
            printf("\n");
            return;

        default:
            printf("Invalid choice\n");
        }
    }
}



void deposit(user *loggedInUser)
{
    float amount;
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);

    if (amount <= 0)
    {
        printf("Invalid Amount\n");
        return;
    }

    loggedInUser->account.balance += amount;
    printf("Amount deposited successfully. New balance: %.2f\n", loggedInUser->account.balance);

    transaction* newTransaction = (transaction*)malloc(sizeof(transaction));
    strcpy(newTransaction->type, "Deposit");
    newTransaction->amount = amount;
    newTransaction->next = loggedInUser->transactions;
    loggedInUser->transactions = newTransaction;

    saveTransactionToFile(loggedInUser, newTransaction);
    updateBalanceAfterTransaction(loggedInUser);

    return;
}



void withdraw(user *loggedInUser)
{
    float amount;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);

    if (amount <= 0 || amount > loggedInUser->account.balance)
    {
        printf("Insufficient balance or invalid amount.\n");
        return;
    }

    loggedInUser->account.balance -= amount;
    printf("Amount withdrawn successfully. New balance: %.2f\n", loggedInUser->account.balance);

    transaction *newTransaction = (transaction *)malloc(sizeof(transaction));
    strcpy(newTransaction->type, "Withdraw");
    newTransaction->amount = amount;
    newTransaction->next = loggedInUser->transactions;
    loggedInUser->transactions = newTransaction;

    saveTransactionToFile(loggedInUser, newTransaction);
    updateBalanceAfterTransaction(loggedInUser);

    return;
}


void transfer(user *loggedInUser)
{
    int receiverAccountNumber;
    float amount;
    printf("Enter receiver account number: ");
    scanf("%d", &receiverAccountNumber);

    if (receiverAccountNumber == loggedInUser->account.accountNumber)
    {
        printf("Cannot transfer amount to the same account.\n");
        return;
    }

    user *receiver = head;
    while (receiver != NULL)
    {
        if (receiver->account.accountNumber == receiverAccountNumber)
        {
            break;
        }
        receiver = receiver->next;
    }

    if (receiver == NULL)
    {
        printf("Receiver account not found\n");
        return;
    }

    if (receiver->status != 'A')
    {
        printf("Cannot transfer to an inactive or deleted account.\n");
        return;
    }

    printf("Enter amount to transfer: ");
    scanf("%f", &amount);

    if (amount <= 0 || amount > loggedInUser->account.balance)
    {
        printf("Insufficient balance or invalid amount.\n");
        return;
    }

    loggedInUser->account.balance -= amount;
    receiver->account.balance += amount;
    printf("Transfer successful. Your new balance: %.2f\n", loggedInUser->account.balance);
    printf("Receiver's new balance: %.2f\n", receiver->account.balance);

    transaction *senderTransaction = (transaction *)malloc(sizeof(transaction));
    strcpy(senderTransaction->type, "Transfer (Sent)");
    senderTransaction->amount = amount;
    senderTransaction->next = loggedInUser->transactions;
    loggedInUser->transactions = senderTransaction;

    transaction *receiverTransaction = (transaction *)malloc(sizeof(transaction));
    strcpy(receiverTransaction->type, "Transfer (Received)");
    receiverTransaction->amount = amount;
    receiverTransaction->next = receiver->transactions;
    receiver->transactions = receiverTransaction;

    saveTransactionToFile(loggedInUser, senderTransaction);
    saveTransactionToFile(receiver, receiverTransaction);
    updateBalanceAfterTransaction(loggedInUser);
    updateBalanceAfterTransaction(receiver);

    return;
}


void checkBalance(user *loggedInUser)
{
    printf("Your current balance: %.2f\n", loggedInUser->account.balance);
    return;
}


void viewTransactionHistory(user *loggedInUser)
{
    if (loggedInUser->transactions == NULL)
    {
        printf("No transactions found.\n");
        return;
    }

    transaction *temp = loggedInUser->transactions;
    printf("Transaction History for %s:\n", loggedInUser->name);
    printf("--------------------------------------------------------------\n");
    printf("Type\t\t\t\tAmount\n");
    printf("--------------------------------------------------------------\n");

    while (temp != NULL)
    {
        printf("%s\t\t\t\t%.2f\n", temp->type, temp->amount);
        temp = temp->next;
    }
    printf("--------------------------------------------------------------\n");
    return;
}






