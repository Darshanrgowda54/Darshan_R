#ifndef ADMIN_H
#define ADMIN_H

typedef struct Admin
{
    char adminID[20];
    char adminPassword[20];
} Admin;


int adminLogin();
void adminMenu();
void createAccount();
void deleteAccount();
void updateAccount();
void viewAccountDetails();
void sortAccountsType();
void displayDeletedAccounts();


#endif
