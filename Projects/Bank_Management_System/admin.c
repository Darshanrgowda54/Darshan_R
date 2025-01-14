#include "admin.h"
#include "fileoperations.h"
#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

user *head = NULL;

int adminLogin()
{
    char adminID[20], adminPassword[20];

    printf("Enter Admin ID: ");
    scanf("%s", adminID);
    printf("Enter Admin Password: ");
    scanf("%s", adminPassword);

    if (strcmp(adminID, "123") == 0 && strcmp(adminPassword, "123") == 0)
    {
        return 1;
    }
    return 0;
}


enum adminMenuOption
{
    CREATACCOUNT = 1,
    DELETEACCOUNT,
    UPDATEACCOUNT,
    VIEWACCOUNTDETAILS,
    SORTACCOUNTSTYPE,
    DISPLAYDELETEDACCOUNTS,
    LOGOUT
};


void adminMenu()
{
    int choice;

    while (1) {
        printf("\n________ Admin Menu ________\n");
        printf("1. Create Account\n");
        printf("2. Delete Account\n");
        printf("3. Update Account\n");
        printf("4. view Account Details\n");
        printf("5. Sort Account Type\n");
        printf("6. Display Deleted Accounts\n");
        printf("7. Logout from Admin\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case CREATACCOUNT:
            createAccount();
            break;

        case DELETEACCOUNT:
            deleteAccount();
            break;

        case UPDATEACCOUNT:
            updateAccount();
            break;

        case VIEWACCOUNTDETAILS:
            viewAccountDetails();
            break;

        case SORTACCOUNTSTYPE:
            sortAccountsType();
            break;

        case DISPLAYDELETEDACCOUNTS:
            displayDeletedAccounts();
            break;

        case LOGOUT:
            printf("________ Logout from Admin ________\n");
            printf("\n");
            return;

        default:
            printf("Invalid choice. Please try again\n");
        }
    }
    return ;
}



int accountCounter = 1001;

void createAccount()
{
    user *newuser = (user*)malloc(sizeof(user));
    if(newuser == NULL)
    {
        printf("Memory Allocation Failed\n");
        return;
    }

    printf("Enter User ID: ");
    scanf(" %[^\n]", newuser->userID);
    printf("Enter Password: ");
    scanf(" %[^\n]", newuser->userPassword);
    printf("Enter Name: ");
    scanf(" %[^\n]", newuser->name);
    printf("Enter Mobile Number: ");
    scanf(" %[^\n]", newuser->mobile);
    printf("Enter Email: ");
    scanf("%s", newuser->email);
    printf("Enter Address: ");
    scanf(" %[^\n]", newuser->address);
    newuser->status ='A';
    newuser->next = NULL;


    while (1)
    {
        printf("Enter Account Type (Savings/Current): ");
        scanf("%s", newuser->account.accountType);
        if (strcmp(newuser->account.accountType, "Savings") == 0 ||strcmp(newuser->account.accountType, "Current") == 0)
        {
            break;
        }
        else
        {
            printf("Invalid Account Type. Please enter 'Savings' or 'Current'.\n");
        }
    }


    while (1)
    {
        int Duplicate = 0;
        user *temp = head;

        while (temp != NULL)
        {
            if (temp->account.accountNumber == accountCounter)
            {
                Duplicate = 1;
                break;
            }
            temp = temp->next;
        }

        if (Duplicate == 0)
        {
            newuser->account.accountNumber = accountCounter++;
            break;
        }
        else
        {
            accountCounter++;
        }
    }

    newuser->account.balance = 0.00;

    newuser->transactions = NULL;

    if(head == NULL)
    {
        head = newuser;
    }
    else
    {
        user *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newuser;
    }

    saveAccountToFile(newuser);

    printf("Account created successfully with Account Number: %d\n", newuser->account.accountNumber);
    return;
}


void deleteAccount()
{
    user *temp = head;
    user *previous = NULL;
    int accountNumber;

    printf("Enter Account Number to Delete: ");
    scanf("%d", &accountNumber);

    while(temp != NULL)
    {
        if(temp->account.accountNumber == accountNumber)
        {
            if (previous == NULL)
            {
                head = temp->next;
            }
            else
            {
                previous->next = temp->next;
            }

            temp->status = 'D';
            deleteAccountInFile(accountNumber);

            //free(temp);
            printf("Account Number %d deleted successfully.\n", accountNumber);
            return;
        }
        previous = temp;
        temp = temp->next;
    }
    printf("Account Number %d not found.\n", accountNumber);
    return;
}


enum  updateAccountMenu{
    NAME = 1,
    MOBILENUMBER,
    EMAIL,
    ADDRESS,
    EXIT
} ;


void updateAccount()
{
    user *temp = head;
    int accountNumber;
    int choice;

    printf("Enter Account Number to Update: ");
    scanf("%d", &accountNumber);

    while (temp !=NULL)
    {
        if(temp->account.accountNumber == accountNumber)
        {
            if (temp->status != 'A')
            {
                printf("This account is not active and cannot be updated.\n");
                return;
            }
            while (1)
            {
                printf("Select the field you want to update\n");
                printf("1. Name\n");
                printf("2. Mobile Number\n");
                printf("3. Email\n");
                printf("4. Address\n");
                printf("5. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice)
                {
                case NAME:
                    printf("Enter New Name: ");
                    scanf(" %[^\n]", temp->name);
                    updateAccountInFile(temp->account.accountNumber, NAME, temp->name);
                    printf("Name updated successfully.\n");
                    break;
                case MOBILENUMBER:
                    printf("Enter New Mobile Number: ");
                    scanf(" %[^\n]", temp->mobile);
                    updateAccountInFile(temp->account.accountNumber, MOBILENUMBER, temp->mobile);
                    printf("Mobile number updated successfully.\n");
                    break;
                case EMAIL:
                    printf("Enter New Email: ");
                    scanf("%s", temp->email);
                    updateAccountInFile(temp->account.accountNumber, EMAIL, temp->email);
                    printf("Email updated successfully.\n");
                    break;
                case ADDRESS:
                    printf("Enter New Address: ");
                    scanf(" %[^\n]", temp->address);
                    updateAccountInFile(temp->account.accountNumber, ADDRESS, temp->address);
                    printf("Address updated successfully.\n");
                    break;
                case EXIT:
                    printf("Exiting update menu\n\n");
                    return;
                default:
                    printf("Invalid choice. Please try again.\n");
                }
            }
        }
        temp = temp->next;
    }
    printf("Account not found.\n");
    return;
}


void viewAccountDetails()
{
    user *temp = head;
    int accountNumber;

    printf("Enter Account Number: ");
    scanf("%d", &accountNumber);

    while(temp != NULL)
    {
        if(temp->account.accountNumber == accountNumber && temp->status== 'A')
        {
            printf("\n________ Account Details ________\n");
            printf("Account Number: %d\n", temp->account.accountNumber);
            printf("Name: %s\n", temp->name);
            printf("Mobile: %s\n", temp->mobile);
            printf("Email: %s\n", temp->email);
            printf("Address: %s\n", temp->address);
            printf("Account Type: %s\n", temp->account.accountType);
            printf("Balance: %.2f\n", temp->account.balance);
            return;
        }
        temp = temp->next;
    }
    printf("Account not found.\n");
    return;
}


user *mergeSortedLists(user *a, user *b)
{
    while (a != NULL && a->status != 'A')
    {
        a = a->next;
    }
    while (b != NULL && b->status != 'A')
    {
        b = b->next;
    }

    if (a == NULL)
        return b;
    if (b == NULL)
        return a;

    if (strcmp(a->account.accountType, b->account.accountType) <= 0)
    {
        a->next = mergeSortedLists(a->next, b);
        return a;
    } else
    {
        b->next = mergeSortedLists(a, b->next);
        return b;
    }
}

user *mergeSort(user *h)
{
    if (h == NULL || h->next == NULL)
    return h;

    user *fast = h->next;
    user *slow = h;
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    user *mid = slow->next;
    slow->next = NULL;

    return mergeSortedLists(mergeSort(h), mergeSort(mid));
}

void sortAccountsType()
{
    if (head == NULL || head->next == NULL)
    {
        printf("No accounts found or only one account exists. Sorting not required.\n");
        return;
    }
    user *filteredHead = NULL, *filteredTail = NULL, *temp = head;
    while (temp != NULL)
    {
        if (temp->status == 'A')
        {
            if (filteredHead == NULL)
            {
                filteredHead = temp;
                filteredTail = temp;
            } else
            {
                filteredTail->next = temp;
                filteredTail = temp;
            }
        }
        temp = temp->next;
    }
    if (filteredTail != NULL)
    {
        filteredTail->next = NULL;
    }

    head = mergeSort(filteredHead);

    printf("________ Accounts Sorted by Account Type ________\n");
    temp = head;
    while (temp != NULL) {
        printf("Account Number: %d\n", temp->account.accountNumber);
        printf("Name: %s\n", temp->name);
        printf("Account Type: %s\n", temp->account.accountType);
        printf("Balance: %.2f\n", temp->account.balance);
        printf("-------------------------------------------\n");
        temp = temp->next;
    }

    printf("Accounts sorted by account type successfully.\n");
}


void displayDeletedAccounts()
{
    user *temp = head;
    int found = 0;

    printf("\nDeleted Accounts:\n");
    printf("----------------------------------\n");

    while (temp != NULL)
    {
        if (temp->status == 'D')
        {
            printf("Account Number: %d\n", temp->account.accountNumber);
            printf("Name: %s\n", temp->name);
            printf("Balance: %.2f\n", temp->account.balance);
            printf("----------------------------------\n");
            found = 1;
        }
        temp = temp->next;
    }

    if (found == 0)
    {
        printf("No deleted accounts found.\n");
    }
}

