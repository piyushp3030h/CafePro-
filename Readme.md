# CafePro – Cafe Billing & Inventory Management System

## About the Project

CafePro is a console-based Cafe Billing and Inventory Management System developed using C programming.

The application simplifies cafe operations by managing menu items, customer records, real-time inventory tracking, bill calculations with discounts, and cumulative sales reports.

## Technologies Used

- C Programming (C99 / C11 Standard)
- GCC Compiler (MinGW-w64)
- File Handling (Binary File Persistence `.dat`)
- Git & GitHub
- GNU Make (`Makefile`)

## Features

- **Product & Inventory Management:** Add, view, search, and restock cafe items.
- **Customer Management:** Register customer profiles with phone validation and lookup.
- **Order & Billing System:** Multi-item cart, live stock checks, discount calculation, and formatted receipts.
- **Sales Analytics:** Aggregated sales volume, discounts, and total net revenue reports.

## Project Structure

- `include/` – Header files (`.h`)
- `src/` – Source code files (`.c`)
- `data/` – Application binary data files (`.dat`)
- `docs/` – Documentation and Project Synopsis PDF
- `main.c` – Main menu and program entry point
- `Makefile` – Build automation

## How to Build and Run

```bash
# Compile using Makefile
mingw32-make

# Run the application
.\cafepro.exe

# Clean build artifacts
mingw32-make clean
```

## Author

- **Piyush Pal**

## Project Status

Completed & Fully Functional.
