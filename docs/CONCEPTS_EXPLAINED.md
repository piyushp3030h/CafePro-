# C Topics Explained (Beginner's Guide to CafePro)

Every concept used in **CafePro** is standard, foundational C programming. Here is a simple breakdown of each topic and where you can see it in the code.

---

## 1. Header Files (`.h`) & Include Guards
* **Where:** `include/product.h`, `include/customer.h`, `include/billing.h`, `include/utils.h`
* **Why:** Header files act like a **table of contents**. They declare structures and function names so `main.c` and other `.c` files know what functions exist without copying code.
* **Include Guards (`#ifndef` / `#define` / `#endif`):**
  ```c
  #ifndef PRODUCT_H
  #define PRODUCT_H
  /* ... declarations ... */
  #endif
  ```
  Prevents compiler errors if the same `.h` file is included more than once.

---

## 2. Constants (`#define`)
* **Where:** `#define MAX_NAME_LEN 50`, `#define PRODUCT_DATA_FILE "data/products.dat"`
* **Why:** Gives a meaningful name to fixed numbers and file paths. If you ever want to change the limit from `50` to `100`, you only change it in one place.

---

## 3. Structures (`struct` and `typedef`)
* **Where:** `Product`, `Customer`, `OrderItem`, `Bill`
* **Why:** Groups related variables of different types into one custom data type.
  ```c
  typedef struct {
      int id;
      char name[50];
      float price;
      int stock_quantity;
  } Product;
  ```
  `typedef` lets us write `Product p;` instead of `struct Product p;`.

---

## 4. Arrays & Strings
* **Where:** `char name[50]`, `OrderItem items[20]`
* **Why:**
  * Strings in C are `char` arrays ending with `'\0'`. We use `strlen()` (length), `strcpy()` (copy), and `strcmp()` (compare two strings).
  * `Bill` holds an array `OrderItem items[20]` so one customer order can contain up to 20 different cafe items.

---

## 5. Functions & Modular Programming
* **Where:** `src/product.c`, `src/customer.c`, `src/billing.c`, `src/utils.c`
* **Why:** Instead of putting 1,000 lines inside `main.c`, each feature lives in its own `.c` file with small, single-purpose functions (like `addProduct`, `generateBill`).

---

## 6. Pointers (`*` and `&`) and Arrow Operator (`->`)
* **Where:** `int addProduct(const Product *product)`, `findProductById(101, &found)`
* **Why:**
  * Passing `&found` (the memory address of `found`) lets a function fill in a `Product` variable for us.
  * When we have a pointer to a `struct` (like `Product *product`), we use `->` to access its fields (`product->id`, `product->price`), whereas a normal struct variable uses `.` (`found.id`).

---

## 7. Control Flow (`while`, `for`, `if/else`, `switch`)
* **Where:** Menu loops and calculations
* **Why:**
  * `while (!feof(stdin))` keeps menus running until the user picks "Exit".
  * `switch (choice)` cleanly routes menu numbers `1`–`5` to the right function.
  * `for (int i = 0; i < bill.item_count; i++)` loops over every item in a bill to compute totals and deduct stock.

---

## 8. Binary File Handling (`FILE *`, `fopen`, `fread`, `fwrite`, `fclose`)
* **Where:** `data/products.dat`, `data/customers.dat`, `data/orders.dat`
* **Why:** Variables in RAM disappear when the program closes. Binary files save entire `struct` blocks directly to disk:
  * `"rb"` — **Read Binary:** Open an existing file to search or display records.
  * `"ab"` — **Append Binary:** Add a new record to the end of the file.
  * `"wb"` — **Write Binary:** Create a fresh file (used for temporary files).
* **How Updating & Deleting Work (Temp File Pattern):**
  Instead of complex in-place file seeking, we open the `.dat` file in `"rb"` and a `.tmp` file in `"wb"`, copy every record (modifying or skipping the target ID), close both, `remove()` the old file, and `rename()` the temp file.

---

## 9. Safe Keyboard Input (`scanf`, `fgets`, `clearInputBuffer`)
* **Where:** `src/utils.c`
* **Why:** Mixing `scanf("%d", ...)` and `fgets(...)` in C can leave a leftover `'\n'` (Enter key) in the input buffer, causing `fgets` to skip. `clearInputBuffer()` reads and discards leftover characters so every prompt works reliably.

