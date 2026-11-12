#ifndef CUSTOMER_H
#define CUSTOMER_H

#define MAX_CUST_NAME_LEN 50
#define MAX_PHONE_LEN 16
#define CUSTOMER_DATA_FILE "data/customers.dat"

typedef struct {
    int id;
    char name[MAX_CUST_NAME_LEN];
    char phone[MAX_PHONE_LEN];
} Customer;

void customerMenu(void);
int addCustomer(const Customer *customer);
void viewCustomers(void);
int findCustomerByPhone(const char *phone, Customer *found);

#endif
