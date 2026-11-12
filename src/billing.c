#include <stdio.h>
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

    printf("Enter Customer Phone (press Enter for Guest): ");
    readString(phone, MAX_PHONE_LEN);
    if (strlen(phone) > 0 && findCustomerByPhone(phone, &cust)) {
        strcpy(bill.customer_name, cust.name);
        printf("Customer found: %s\n", bill.customer_name);
    } else {
        strcpy(bill.customer_name, "Guest");
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
    }
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
