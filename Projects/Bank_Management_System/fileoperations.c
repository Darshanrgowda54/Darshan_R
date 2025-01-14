#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "bankaccount.h"
#include "fileoperations.h"

#define ACCOUNT_FILE "acc.txt"
#define TRANSACTION_FILE "tra.txt"


void saveTransactionToFile(user *currentUser, transaction *newTransaction)
{
    FILE *file = fopen(TRANSACTION_FILE, "a+");
    if (file == NULL)
    {
        printf("Error opening %s\n", TRANSACTION_FILE);
        return;
    }

    fprintf(file, "%d,%s,%.2f\n",currentUser->account.accountNumber, newTransaction->type,newTransaction->amount);

    fflush(file);
    fclose(file);
}



void loadTransactionsFromFile() {
    FILE *file = fopen(TRANSACTION_FILE, "r");
    if (file == NULL)
    {
        printf("No existing transactions found.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        int accountNumber;
        transaction *newTransaction = (transaction *)malloc(sizeof(transaction));

        sscanf(line, "%d,%19[^,],%f", &accountNumber, newTransaction->type, &newTransaction->amount);

        user *temp = head;
        while (temp)
        {
            if (temp->account.accountNumber == accountNumber)
            {
                newTransaction->next = temp->transactions;
                temp->transactions = newTransaction;
                break;
            }
            temp = temp->next;
        }
    }

    fclose(file);
}


void updateBalanceAfterTransaction(user *currentUser)
{
    updateAccountInFile(currentUser->account.accountNumber, 5, &currentUser->account.balance);
}




void loadAccountsFromFile() {
    FILE *file = fopen(ACCOUNT_FILE, "r");
    if (file == NULL)
    {
        printf("No existing accounts found.\n");
        return;
    }

    char line[512];
    user *temp = NULL;

    while (fgets(line, sizeof(line), file))
    {
        user *newUser = (user *)malloc(sizeof(user));
        if (newUser == NULL)
        {
            printf("Memory allocation failed for new user.\n");
            fclose(file);
            return;
        }

        sscanf(line, "%10d%10s%10s%24[^\t\n]%12s%25s%30s%10s%f %c",
               &newUser->account.accountNumber,
               newUser->userID,
               newUser->userPassword,
               newUser->name,
               newUser->mobile,
               newUser->email,
               newUser->address,
               newUser->account.accountType,
               &newUser->account.balance,
               &newUser->status);

        // Explicitly trim spaces in the name
        char *start = newUser->name;
        while (isspace((unsigned char)*start)) start++; // Skip leading spaces
        char *end = start + strlen(start) - 1;
        while (end > start && isspace((unsigned char)*end)) end--; // Skip trailing spaces
        *(end + 1) = '\0'; // Null-terminate after trimming

        // Copy the trimmed value back to the structure
        strcpy(newUser->name, start);

        printf("Loaded Account: %d, Balance: %.2f\n", newUser->account.accountNumber, newUser->account.balance);
        newUser->transactions = NULL;
        newUser->next = NULL;

        if (head == NULL)
        {
            head = newUser;
        } else
        {
            temp = head;
            while (temp->next)
            {
                temp = temp->next;
            }
            temp->next = newUser;
        }
    }
    fclose(file);
}



void saveAccountToFile(user *newuser)
{
    FILE *file = fopen(ACCOUNT_FILE, "a+");
    if (file == NULL)
    {
        printf("Error opening %s\n", ACCOUNT_FILE);
        return;
    }
   fseek(file, 0, SEEK_END);
    fprintf(file, "%-10d%-10s%-10s%-25s%-12s%-25s%-30s%-10s%-10.2f%c\n",
            newuser->account.accountNumber,
            newuser->userID,
            newuser->userPassword,
            newuser->name,
            newuser->mobile,
            newuser->email,
            newuser->address,
            newuser->account.accountType,
            newuser->account.balance,
            newuser->status);

    fflush(file);
    fclose(file);
}



void updateAccountInFile(int accountNumber, int fieldToUpdate, void *newValue)
{
    FILE *file = fopen(ACCOUNT_FILE, "r+");
    if (file == NULL)
    {
        printf("Error opening %s for updating.\n", ACCOUNT_FILE);
        return;
    }

    rewind(file);
    char line[512];
    long position = 0;
    int found = 0;

    while (fgets(line, sizeof(line), file))
    {
        int existingAccountNumber;
        sscanf(line, "%d", &existingAccountNumber);

        if (existingAccountNumber == accountNumber)
        {
            found = 1;
            position = ftell(file) - strlen(line);
            fseek(file, position, SEEK_SET);

            switch (fieldToUpdate)
            {
            case 1:
                fseek(file, position + 29, SEEK_SET);
                fprintf(file, "%-25s", (char *)newValue);
                break;
            case 2:
                fseek(file, position + 54, SEEK_SET);
                fprintf(file, "%-12s", (char *)newValue);
                break;
            case 3:
                fseek(file, position + 66, SEEK_SET);
                fprintf(file, "%-25s", (char *)newValue);
                break;
            case 4:
                fseek(file, position + 91, SEEK_SET);
                fprintf(file, "%-30s", (char *)newValue);
                break;
            case 5:
                if (fieldToUpdate == 5)
                {
                    fseek(file, position + 131, SEEK_SET);
                    fprintf(file, "%-10.2f", *(float *)newValue);
                }
                break;
            default:
                printf("Invalid field selected for update.\n");
                break;
            }
            fflush(file);
            break;
        }
    }

    if (found == 0)
    {
        printf("Account with number %d not found.\n", accountNumber);
    }

    fclose(file);
}



void deleteAccountInFile(int accountNumber)
{
    FILE *file = fopen(ACCOUNT_FILE, "r+");
    if (file == NULL)
    {
        printf("Error opening file for deletion.\n");
        return;
    }

    char line[512];
    long position;
    int found = 0;

    while (fgets(line, sizeof(line), file))
    {
        int existingAccountNumber;
        sscanf(line, "%d", &existingAccountNumber);

        if (existingAccountNumber == accountNumber)
        {
            found = 1;
            position = ftell(file) - strlen(line);
            fseek(file, position + 141, SEEK_SET);
            fprintf(file, "D");
            fflush(file);
            break;
        }
    }

    if (found == 0)
    {
        printf("Account with number %d not found.\n", accountNumber);
    }

    fclose(file);
}
