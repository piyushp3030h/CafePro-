#ifndef BILLING_H
#define BILLING_H

#include "product.h"
#include "customer.h"

#define MAX_ITEMS_PER_BILL 20
#define BILLING_DATA_FILE "data/orders.dat"

typedef struct {
    int product_id;
    char product_name[MAX_NAME_LEN];
    int quantity;
    float unit_price;
    float subtotal;
} OrderItem;

typedef struct {
    int bill_id;
    int customer_id;
    char date[20];
    OrderItem items[MAX_ITEMS_PER_BILL];
    int item_count;
    float total_amount;
    float discount;
    float final_amount;
} Bill;

/* Function Declarations - Billing & Order Module */
void billingMenu(void);
void generateBill(void);
void displaySalesReport(void);

#endif /* BILLING_H */

