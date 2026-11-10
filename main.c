#include <stdio.h>
#include <stdlib.h>
#include "product.h"
#include "customer.h"
#include "billing.h"

/* Stubs for friend's modules under development */
void customerMenu(void) {
    printf("\n[Customer Management Module - Assigned to Collaborator]\n");
}

void billingMenu(void) {
    printf("\n[Billing & Orders Module - Assigned to Collaborator]\n");
}

static void displayMainMenu(void) {
    printf("\n=========================================\n");
    printf("        CAFEPRO MANAGEMENT SYSTEM        \n");
    printf("=========================================\n");
    printf(" 1. Product & Inventory Management\n");
    printf(" 2. Customer Management\n");
    printf(" 3. Billing & Order Processing\n");
    printf(" 4. Exit\n");
    printf("=========================================\n");
    printf("Enter your choice (1-4): ");
}

int main(void) {
    int choice = 0;

    while (1) {
        displayMainMenu();
        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input! Please enter a number.\n");
            while (getchar() != '\n'); /* Clear input buffer */
            continue;
        }

        switch (choice) {
            case 1:
                productMenu();
                break;
            case 2:
                customerMenu();
                break;
            case 3:
                billingMenu();
                break;
            case 4:
                printf("\nExiting CafePro. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please select between 1 and 4.\n");
        }
    }

    return 0;
}
