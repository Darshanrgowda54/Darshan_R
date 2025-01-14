#include <stdio.h>
#include <stdlib.h>
#include "admin.h"
#include "fileoperations.h"
#include "user.h"

enum mainMenuOption
{
    ADMIN = 1,
    USER,
    EXIT
};


int mainMenu()
{
    loadAccountsFromFile();
    loadTransactionsFromFile();

    int choice;

    while (1)
    {
        printf("\n________ Bank Management System ________\n");
        printf("1. Admin Login\n");
        printf("2. User Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case ADMIN:
            if (adminLogin())
            {
                printf("Admin login Successful.\n");
                adminMenu();
            }
            else
            {
                printf("Invalid admin credentials.\n");
            }
            break;
        case USER:
            userLogin();
            break;

        case EXIT:
            printf("Thank you. Please visit again\n");
            free(head);
            return 0;

        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    }
}
