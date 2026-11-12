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
    char customer_name[MAX_CUST_NAME_LEN];
    OrderItem items[MAX_ITEMS_PER_BILL];
    int item_count;
    float total_amount;
    float discount;
    float final_amount;
} Bill;

void billingMenu(void);
int saveBill(const Bill *bill);
void createBill(void);
void viewBills(void);
void viewSalesReport(void);

#endif
