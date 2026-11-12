#include <stdio.h>
#include "product.h"
#include "customer.h"
#include "billing.h"
#include "utils.h"

int main(void) {
    int choice = 0;

    while (!feof(stdin)) {
        printf("\n=========================================\n");
        printf("        CAFEPRO MANAGEMENT SYSTEM        \n");
        printf("=========================================\n");
        printf("1. Product & Inventory\n");
        printf("2. Customer Management\n");
        printf("3. Billing & Sales\n");
        printf("4. Exit\n");
        printf("=========================================\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

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
                return 0;
            default:
                printf("\nInvalid choice! Pick 1 to 4.\n");
        }
    }

    return 0;
}
