#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "billing.h"
#include "utils.h"

int saveBill(const Bill *bill) {
    if (bill == NULL || bill->bill_id <= 0 || bill->item_count <= 0) {
        return 0;
    }

    FILE *fp = fopen(BILLING_DATA_FILE, "ab");
    if (fp == NULL) {
        printf("\n[Error] Could not open %s.\n", BILLING_DATA_FILE);
        return 0;
    }

    int written = (int)fwrite(bill, sizeof(Bill), 1, fp);
    fclose(fp);
    return (written == 1) ? 1 : 0;
}

void createBill(void) {
    Bill bill = {0};
    char phone[MAX_PHONE_LEN];
    Customer cust;

    printf("\nEnter Bill ID (e.g. 5001): ");
    if (scanf("%d", &bill.bill_id) != 1 || bill.bill_id <= 0) {
        clearInputBuffer();
        printf("[Error] Invalid Bill ID.\n");
        return;
    }
    clearInputBuffer();

    printf("Enter Customer Phone (press Enter or 0 for Guest): ");
    readString(phone, MAX_PHONE_LEN);

    if (strlen(phone) == 0 || strcmp(phone, "0") == 0) {
        strcpy(bill.customer_name, "Guest");
        printf("Proceeding as Guest customer.\n");
    } else if (findCustomerByPhone(phone, &cust)) {
        strcpy(bill.customer_name, cust.name);
        printf("[Member Found] Welcome back, %s! (ID: %d)\n", bill.customer_name, cust.id);
    } else {
        printf("[Notice] Phone '%s' is not registered in our customer directory.\n", phone);
        printf("Would you like to register this customer now?\n");
        printf("  1. Yes, register customer\n");
        printf("  2. No, continue as Guest\n");
        printf("Enter choice (1 or 2): ");
        int reg_choice = 2;
        if (scanf("%d", &reg_choice) != 1) {
            clearInputBuffer();
            reg_choice = 2;
        } else {
            clearInputBuffer();
        }

        if (reg_choice == 1) {
            Customer new_cust;
            new_cust.id = getNextCustomerId();
            strcpy(new_cust.phone, phone);
            printf("Enter Customer Name: ");
            readString(new_cust.name, MAX_CUST_NAME_LEN);

            if (strlen(new_cust.name) > 0 && addCustomer(&new_cust)) {
                strcpy(bill.customer_name, new_cust.name);
                printf("[Success] Customer '%s' (ID: %d) registered and linked to this bill!\n",
                       bill.customer_name, new_cust.id);
            } else {
                strcpy(bill.customer_name, "Guest");
                printf("[Notice] Registration skipped; continuing as Guest.\n");
            }
        } else {
            strcpy(bill.customer_name, "Guest");
            printf("Continuing as Guest customer.\n");
        }
    }

    viewProducts();

    while (bill.item_count < MAX_ITEMS_PER_BILL && !feof(stdin)) {
        int prod_id, qty;
        Product prod;

        printf("\nEnter Product ID to add (0 to finish): ");
        if (scanf("%d", &prod_id) != 1) {
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        if (prod_id == 0) {
            break;
        }
        if (!findProductById(prod_id, &prod)) {
            printf("[Error] Product ID %d not found.\n", prod_id);
            continue;
        }

        printf("Selected: %s ($%.2f) | Available stock: %d\n",
               prod.name, prod.price, prod.stock_quantity);
        printf("Enter Quantity: ");
        if (scanf("%d", &qty) != 1 || qty <= 0) {
            clearInputBuffer();
            printf("[Error] Invalid quantity.\n");
            continue;
        }
        clearInputBuffer();

        if (!updateProductStock(prod.id, -qty)) {
            continue;
        }

        OrderItem *item = &bill.items[bill.item_count];
        item->product_id = prod.id;
        strcpy(item->product_name, prod.name);
        item->quantity = qty;
        item->unit_price = prod.price;
        item->subtotal = qty * prod.price;

        bill.total_amount += item->subtotal;
        bill.item_count++;
        printf("[Added] %dx %s\n", qty, prod.name);
    }

    if (bill.item_count == 0) {
        printf("\n[Notice] Bill cancelled (no items added).\n");
        return;
    }

    float disc_pct = 0.0f;
    printf("\nSubtotal: $%.2f\n", bill.total_amount);
    printf("Enter Discount %% (0 for none): ");
    if (scanf("%f", &disc_pct) == 1 && disc_pct >= 0.0f && disc_pct <= 100.0f) {
        bill.discount = bill.total_amount * (disc_pct / 100.0f);
    }
    clearInputBuffer();

    bill.final_amount = bill.total_amount - bill.discount;

    if (saveBill(&bill)) {
        printf("\n================ CAFEPRO RECEIPT ================\n");
        printf("Bill ID: #%d | Customer: %s\n", bill.bill_id, bill.customer_name);
        printf("-------------------------------------------------\n");
        for (int i = 0; i < bill.item_count; i++) {
            printf("%-20s x%-3d  @ $%6.2f = $%7.2f\n",
                   bill.items[i].product_name,
                   bill.items[i].quantity,
                   bill.items[i].unit_price,
                   bill.items[i].subtotal);
        }
        printf("-------------------------------------------------\n");
        printf("Total:    $%.2f\n", bill.total_amount);
        printf("Discount: -$%.2f\n", bill.discount);
        printf("PAYABLE:  $%.2f\n", bill.final_amount);
        printf("=================================================\n");

        printf("\nPrint receipt to printer? (1: Yes, 2: Skip): ");
        int prn_choice = 2;
        if (scanf("%d", &prn_choice) == 1) {
            clearInputBuffer();
        } else {
            clearInputBuffer();
            prn_choice = 2;
        }

        if (prn_choice == 1) {
            printReceiptToPrinter(&bill);
        }
    }
}

void printReceiptToPrinter(const Bill *bill) {
    if (bill == NULL) {
        return;
    }

    const char *receipt_file = "data/receipt.txt";
    FILE *fp = fopen(receipt_file, "w");
    if (fp == NULL) {
        printf("[Error] Could not generate receipt file for printing.\n");
        return;
    }

    fprintf(fp, "=================================================\n");
    fprintf(fp, "                     CAFEPRO                     \n");
    fprintf(fp, "         CAFE BILLING & INVENTORY SYSTEM         \n");
    fprintf(fp, "=================================================\n");
    fprintf(fp, "Bill ID  : #%d\n", bill->bill_id);
    fprintf(fp, "Customer : %s\n", bill->customer_name);
    fprintf(fp, "-------------------------------------------------\n");
    fprintf(fp, "%-20s %-5s   %8s   %9s\n", "Item", "Qty", "Price", "Subtotal");
    fprintf(fp, "-------------------------------------------------\n");

    for (int i = 0; i < bill->item_count; i++) {
        fprintf(fp, "%-20s x%-4d  $%7.2f  $%8.2f\n",
                bill->items[i].product_name,
                bill->items[i].quantity,
                bill->items[i].unit_price,
                bill->items[i].subtotal);
    }

    fprintf(fp, "-------------------------------------------------\n");
    fprintf(fp, "Total Amount : $%8.2f\n", bill->total_amount);
    fprintf(fp, "Discount     : -$%7.2f\n", bill->discount);
    fprintf(fp, "FINAL PAYABLE: $%8.2f\n", bill->final_amount);
    fprintf(fp, "=================================================\n");
    fprintf(fp, "            Thank you for your visit!            \n");
    fprintf(fp, "=================================================\n\n\n\n");
    fclose(fp);

    printf("[Printing] Sending receipt to Windows Default Printer...\n");
    system("notepad /p data\\receipt.txt");
    printf("[Success] Print job queued successfully!\n");
}

void viewBills(void) {
    FILE *fp = fopen(BILLING_DATA_FILE, "rb");
    if (fp == NULL) {
        printf("\n[Notice] No bills recorded yet.\n");
        return;
    }

    Bill b;
    printf("\n=========================================================\n");
    printf("                      ORDER HISTORY                      \n");
    printf("=========================================================\n");
    printf("%-8s | %-20s | %-6s | %-10s\n", "Bill #", "Customer", "Items", "Final Amt");
    printf("---------+----------------------+--------+---------------\n");

    while (fread(&b, sizeof(Bill), 1, fp) == 1) {
        printf("%-8d | %-20s | %-6d | $%9.2f\n",
               b.bill_id, b.customer_name, b.item_count, b.final_amount);
    }
    printf("=========================================================\n");
    fclose(fp);
}

void viewSalesReport(void) {
    FILE *fp = fopen(BILLING_DATA_FILE, "rb");
    if (fp == NULL) {
        printf("\n[Notice] No sales data yet.\n");
        return;
    }

    Bill b;
    int total_orders = 0;
    float total_revenue = 0.0f;

    while (fread(&b, sizeof(Bill), 1, fp) == 1) {
        total_orders++;
        total_revenue += b.final_amount;
    }
    fclose(fp);

    printf("\n=== CAFEPRO SALES REPORT ===\n");
    printf("Total Orders  : %d\n", total_orders);
    printf("Total Revenue : $%.2f\n", total_revenue);
    printf("============================\n");
}

void billingMenu(void) {
    int choice = 0;

    while (!feof(stdin)) {
        printf("\n=== BILLING & SALES ===\n");
        printf("1. Create New Bill\n");
        printf("2. View All Bills\n");
        printf("3. Sales Report\n");
        printf("4. Back to Main Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        if (choice == 1) {
            createBill();
        } else if (choice == 2) {
            viewBills();
        } else if (choice == 3) {
            viewSalesReport();
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice! Pick 1 to 4.\n");
        }
    }
}
