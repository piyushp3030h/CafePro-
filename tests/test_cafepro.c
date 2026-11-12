#include <stdio.h>
#include <string.h>
#include "product.h"
#include "customer.h"
#include "billing.h"

static int passed = 0;
static int failed = 0;

static void check(int condition, const char *name) {
    if (condition) {
        passed++;
    } else {
        printf("  [FAIL] %s\n", name);
        failed++;
    }
}

static void cleanData(void) {
    remove(PRODUCT_DATA_FILE);
    remove(CUSTOMER_DATA_FILE);
    remove(BILLING_DATA_FILE);
}

int main(void) {
    cleanData();

    /* Product tests */
    Product p = {101, "Espresso", "Beverage", 3.50f, 20};
    Product found_p;
    check(addProduct(&p) == 1, "Add product");
    check(addProduct(&p) == 0, "Reject duplicate product ID");
    check(findProductById(101, &found_p) == 1, "Find product by ID");
    check(updateProductStock(101, -5) == 1, "Deduct 5 stock");
    check(findProductById(101, &found_p) == 1 && found_p.stock_quantity == 15, "Stock is 15");
    check(updateProductStock(101, -20) == 0, "Block negative stock");

    /* Customer tests */
    Customer c = {1, "Alice", "9876543210"};
    Customer found_c;
    check(addCustomer(&c) == 1, "Add customer");
    check(addCustomer(&c) == 0, "Reject duplicate phone");
    check(findCustomerByPhone("9876543210", &found_c) == 1, "Find customer by phone");
    check(strcmp(found_c.name, "Alice") == 0, "Verify customer name");

    /* Billing tests */
    Bill b = {0};
    b.bill_id = 5001;
    strcpy(b.customer_name, "Alice");
    b.item_count = 1;
    b.items[0].product_id = 101;
    strcpy(b.items[0].product_name, "Espresso");
    b.items[0].quantity = 2;
    b.items[0].unit_price = 3.50f;
    b.items[0].subtotal = 7.00f;
    b.total_amount = 7.00f;
    b.final_amount = 7.00f;
    check(saveBill(&b) == 1, "Save bill");

    cleanData();

    printf("\nTests Passed: %d | Tests Failed: %d\n", passed, failed);
    return (failed == 0) ? 0 : 1;
}
