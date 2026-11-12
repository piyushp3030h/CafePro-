# CafePro — Project Documentation

## 1. Project Overview
**CafePro** is a console-based Cafe Billing and Inventory Management System written in standard C. It provides simple, persistent record-keeping using binary files (`.dat`) without needing external databases.

---

## 2. Folder Structure
```text
CafePro/
├── include/     # Header files (.h) defining structs and function declarations
├── src/         # Implementation files (.c) for each feature
├── data/        # Binary files (.dat) storing runtime records
├── tests/       # Automated test suite (test_cafepro.c)
├── docs/        # Documentation and learning guides
├── main.c       # Main menu and program entry point
└── Makefile     # Build script for GCC
```

---

## 3. Modules & Responsibilities
| Module | Header | Source | Main Functions |
| :--- | :--- | :--- | :--- |
| **Product** | `product.h` | `product.c` | `addProduct`, `viewProducts`, `findProductById`, `updateProductStock`, `productMenu` |
| **Customer** | `customer.h` | `customer.c` | `addCustomer`, `viewCustomers`, `findCustomerByPhone`, `customerMenu` |
| **Billing** | `billing.h` | `billing.c` | `createBill`, `saveBill`, `viewBills`, `viewSalesReport`, `billingMenu` |
| **Utilities** | `utils.h` | `utils.c` | `clearInputBuffer`, `readString` |

---

## 4. Data Models (Structures)
* **Product:** ID, Name, Category, Price, Stock Quantity.
* **Customer:** ID, Name, Phone.
* **OrderItem:** Product ID, Name, Quantity, Unit Price, Subtotal.
* **Bill:** Bill ID, Customer Name, Items Array (`OrderItem[20]`), Count, Total, Discount, Final Amount.

---

## 5. Storage (Binary Files)
All data is stored directly as binary structs:
* `data/products.dat`
* `data/customers.dat`
* `data/orders.dat`

*Writing:* `fopen("...", "ab")` + `fwrite()`  
*Reading:* `fopen("...", "rb")` + `fread()` in a `while` loop  
*Updating:* Read `.dat` file, write updated records to `.tmp`, delete old file, rename `.tmp` to `.dat`.

---

## 6. How to Build and Run
```bash
# Compile the main project
mingw32-make

# Run the application
.\cafepro.exe

# Run automated tests
mingw32-make test

# Clean compiled executables
mingw32-make clean
```

