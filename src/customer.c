#include <stdio.h>
#include <string.h>
#include "customer.h"
#include "utils.h"

int addCustomer(const Customer *customer) {
    if (customer == NULL || customer->id <= 0 || strlen(customer->phone) == 0) {
        return 0;
    }
    if (findCustomerByPhone(customer->phone, NULL)) {
        return 0;
    }

    FILE *fp = fopen(CUSTOMER_DATA_FILE, "ab");
    if (fp == NULL) {
        printf("\n[Error] Could not open %s.\n", CUSTOMER_DATA_FILE);
        return 0;
    }

    int written = (int)fwrite(customer, sizeof(Customer), 1, fp);
    fclose(fp);
    return (written == 1) ? 1 : 0;
}

void viewCustomers(void) {
    FILE *fp = fopen(CUSTOMER_DATA_FILE, "rb");
    if (fp == NULL) {
        printf("\n[Notice] No customers registered yet.\n");
        return;
    }

    Customer c;
    printf("\n====================================================\n");
    printf("                REGISTERED CUSTOMERS                \n");
    printf("====================================================\n");
    printf("%-6s | %-24s | %-15s\n", "ID", "Name", "Phone");
    printf("-------+--------------------------+-----------------\n");

    while (fread(&c, sizeof(Customer), 1, fp) == 1) {
        printf("%-6d | %-24s | %-15s\n", c.id, c.name, c.phone);
    }
    printf("====================================================\n");
    fclose(fp);
}

int findCustomerByPhone(const char *phone, Customer *found) {
    if (phone == NULL || strlen(phone) == 0) {
        return 0;
    }

    FILE *fp = fopen(CUSTOMER_DATA_FILE, "rb");
    if (fp == NULL) {
        return 0;
    }

    Customer temp;
    while (fread(&temp, sizeof(Customer), 1, fp) == 1) {
        if (strcmp(temp.phone, phone) == 0) {
            if (found != NULL) {
                *found = temp;
            }
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void customerMenu(void) {
    int choice = 0;

    while (!feof(stdin)) {
        printf("\n=== CUSTOMER MANAGEMENT ===\n");
        printf("1. Add Customer\n");
        printf("2. View All Customers\n");
        printf("3. Search by Phone\n");
        printf("4. Back to Main Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        if (choice == 1) {
            Customer c;
            printf("Enter Customer ID: ");
            if (scanf("%d", &c.id) != 1) {
                clearInputBuffer();
                continue;
            }
            clearInputBuffer();

            printf("Enter Customer Name: ");
            readString(c.name, MAX_CUST_NAME_LEN);
            printf("Enter Phone Number: ");
            readString(c.phone, MAX_PHONE_LEN);

            if (addCustomer(&c)) {
                printf("[Success] Customer '%s' added!\n", c.name);
            } else {
                printf("[Error] Could not add customer (duplicate phone or invalid ID).\n");
            }
        } else if (choice == 2) {
            viewCustomers();
        } else if (choice == 3) {
            char phone[MAX_PHONE_LEN];
            Customer found;
            printf("Enter Phone Number: ");
            readString(phone, MAX_PHONE_LEN);

            if (findCustomerByPhone(phone, &found)) {
                printf("\nFound: ID %d | Name: %s | Phone: %s\n",
                       found.id, found.name, found.phone);
            } else {
                printf("[Notice] No customer found with phone '%s'.\n", phone);
            }
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice! Pick 1 to 4.\n");
        }
    }
}
