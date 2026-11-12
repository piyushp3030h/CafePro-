# CafePro Architecture & Module Guide

## Folder Structure

- `include/` — Header files (`.h`) defining shared structs, constants, and public function declarations.
- `src/` — Modular C implementation files (`.c`).
- `data/` — Runtime binary data files (`.dat`), ignored by Git.
- `tests/` — Unit and integration tests for verifying modules independently.
- `docs/` — Architecture and design documentation.

## Modules

1. **Utilities (`include/utils.h`, `src/utils.c`)**
   - Safe console input (`readIntInput`, `readFloatInput`, `readStringInput`, `clearInputBuffer`).
   - Timestamp formatting (`getCurrentDateTime`) and directory initialization (`ensureDataDirectory`).

2. **Product & Inventory (`include/product.h`, `src/product.c`)**
   - Storage: `data/products.dat` (fixed-size binary `Product` records).
   - Capabilities: Add product (duplicate ID guarded), view inventory table, search by ID, restock/deduct inventory (negative stock guarded), edit details, delete via temp-file rewrite, low-stock alerts (`<= 5`).

3. **Customer Management (`include/customer.h`, `src/customer.c`)**
   - Storage: `data/customers.dat` (fixed-size binary `Customer` records).
   - Capabilities: Register customer (unique ID & phone), list customers, search by phone/ID, loyalty points updates.

4. **Billing & Orders (`include/billing.h`, `src/billing.c`)**
   - Storage: `data/orders.dat` (fixed-size binary `Bill` records containing up to `20` `OrderItem` entries).
   - Capabilities: Multi-item cart with live stock validation and duplicate-item merging, optional customer linking, discount percentage calculation, automatic stock deduction with rollback on failure, loyalty point rewards (1 pt per $10 spent), receipt printing, order history, and sales summary report.

## Build & Test Commands

```bash
# Build application
mingw32-make

# Run unit tests
mingw32-make test

# Clean executables
mingw32-make clean
```

