#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product.h"

/*
 * addProduct:
 * Appends a new product record to the binary data file.
 * Returns 1 on success, 0 on failure.
 */
int addProduct(const Product *product) {
    if (product == NULL) {
        return 0;
    }

    FILE *fp = fopen(PRODUCT_DATA_FILE, "ab");
    if (fp == NULL) {
        perror("Error opening product data file for writing");
        return 0;
    }

    size_t written = fwrite(product, sizeof(Product), 1, fp);
    fclose(fp);

    return (written == 1) ? 1 : 0;
}

/*
 * displayAllProducts:
 * Reads and displays all products stored in the binary data file
 * in a clean, formatted table.
 */
void displayAllProducts(void) {
    FILE *fp = fopen(PRODUCT_DATA_FILE, "rb");
    if (fp == NULL) {
        printf("\n[Notice] No products found in inventory yet.\n");
        return;
    }

    Product p;
    int count = 0;

    printf("\n====================================================================\n");
    printf("                       PRODUCT INVENTORY                            \n");
    printf("====================================================================\n");
    printf("%-6s | %-22s | %-15s | %-8s | %-6s\n", "ID", "Name", "Category", "Price", "Stock");
    printf("-------+------------------------+-----------------+----------+------\n");

    while (fread(&p, sizeof(Product), 1, fp) == 1) {
        printf("%-6d | %-22s | %-15s | %8.2f | %-6d\n",
               p.id, p.name, p.category, p.price, p.stock_quantity);
        count++;
    }

    printf("====================================================================\n");
    printf(" Total Products: %d\n", count);
    printf("====================================================================\n");

    fclose(fp);
}

/*
 * findProductById:
 * Searches for a product by its unique ID.
 * Populates *found and returns 1 if found, 0 otherwise.
 */
int findProductById(int id, Product *found) {
    FILE *fp = fopen(PRODUCT_DATA_FILE, "rb");
    if (fp == NULL) {
        return 0;
    }

    Product temp;
    int match = 0;

    while (fread(&temp, sizeof(Product), 1, fp) == 1) {
        if (temp.id == id) {
            if (found != NULL) {
                *found = temp;
            }
            match = 1;
            break;
        }
    }

    fclose(fp);
    return match;
}

/*
 * updateProductStock:
 * Updates the stock of a product by adding quantity_change (+ for restock, - for sale).
 * Returns 1 on success, 0 on failure.
 */
int updateProductStock(int id, int quantity_change) {
    FILE *fp = fopen(PRODUCT_DATA_FILE, "rb+");
    if (fp == NULL) {
        return 0;
    }

    Product temp;
    int updated = 0;

    while (fread(&temp, sizeof(Product), 1, fp) == 1) {
        if (temp.id == id) {
            /* Prevent negative inventory */
            if (temp.stock_quantity + quantity_change < 0) {
                printf("\n[Error] Insufficient stock! Available: %d, Requested change: %d\n",
                       temp.stock_quantity, quantity_change);
                fclose(fp);
                return 0;
            }

            temp.stock_quantity += quantity_change;

            /* Move file pointer back one record to overwrite */
            fseek(fp, -(long)sizeof(Product), SEEK_CUR);
            fwrite(&temp, sizeof(Product), 1, fp);
            updated = 1;
            break;
        }
    }

    fclose(fp);
    return updated;
}

/* Helper to safely read a string from stdin */
static void readStringInput(char *buffer, int max_len) {
    if (fgets(buffer, max_len, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

/*
 * productMenu:
 * Interactive sub-menu loop for managing cafe products and inventory.
 */
void productMenu(void) {
    int choice = 0;

    while (1) {
        printf("\n=========================================\n");
        printf("       PRODUCT & INVENTORY MENU          \n");
        printf("=========================================\n");
        printf(" 1. Add New Product\n");
        printf(" 2. View All Products\n");
        printf(" 3. Search Product by ID\n");
        printf(" 4. Update Product Stock (Restock)\n");
        printf(" 5. Back to Main Menu\n");
        printf("=========================================\n");
        printf("Enter your choice (1-5): ");

        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n'); /* consume newline */

        if (choice == 1) {
            Product new_prod;
            printf("\nEnter Product ID (integer): ");
            if (scanf("%d", &new_prod.id) != 1) {
                printf("Invalid ID format!\n");
                while (getchar() != '\n');
                continue;
            }
            while (getchar() != '\n');

            /* Check duplicate ID */
            if (findProductById(new_prod.id, NULL)) {
                printf("\n[Error] A product with ID %d already exists!\n", new_prod.id);
                continue;
            }

            printf("Enter Product Name (e.g. Espresso, Croissant): ");
            readStringInput(new_prod.name, MAX_NAME_LEN);

            printf("Enter Category (e.g. Beverage, Bakery, Snack): ");
            readStringInput(new_prod.category, MAX_CATEGORY_LEN);

            printf("Enter Price: ");
            if (scanf("%f", &new_prod.price) != 1 || new_prod.price < 0) {
                printf("Invalid price! Price must be non-negative.\n");
                while (getchar() != '\n');
                continue;
            }

            printf("Enter Initial Stock Quantity: ");
            if (scanf("%d", &new_prod.stock_quantity) != 1 || new_prod.stock_quantity < 0) {
                printf("Invalid stock quantity!\n");
                while (getchar() != '\n');
                continue;
            }
            while (getchar() != '\n');

            if (addProduct(&new_prod)) {
                printf("\n[Success] Product '%s' (ID: %d) added successfully!\n",
                       new_prod.name, new_prod.id);
            } else {
                printf("\n[Failure] Failed to save product.\n");
            }

        } else if (choice == 2) {
            displayAllProducts();

        } else if (choice == 3) {
            int search_id;
            printf("\nEnter Product ID to search: ");
            if (scanf("%d", &search_id) != 1) {
                printf("Invalid ID!\n");
                while (getchar() != '\n');
                continue;
            }
            while (getchar() != '\n');

            Product found;
            if (findProductById(search_id, &found)) {
                printf("\nProduct Found:\n");
                printf("  ID:       %d\n", found.id);
                printf("  Name:     %s\n", found.name);
                printf("  Category: %s\n", found.category);
                printf("  Price:    $%.2f\n", found.price);
                printf("  Stock:    %d units\n", found.stock_quantity);
            } else {
                printf("\n[Notice] No product found with ID %d.\n", search_id);
            }

        } else if (choice == 4) {
            int id, qty;
            printf("\nEnter Product ID to update stock: ");
            if (scanf("%d", &id) != 1) {
                printf("Invalid ID!\n");
                while (getchar() != '\n');
                continue;
            }
            printf("Enter Quantity to Add (positive number): ");
            if (scanf("%d", &qty) != 1 || qty <= 0) {
                printf("Invalid quantity! Must be greater than 0.\n");
                while (getchar() != '\n');
                continue;
            }
            while (getchar() != '\n');

            if (updateProductStock(id, qty)) {
                printf("\n[Success] Stock updated successfully!\n");
            } else {
                printf("\n[Failure] Product ID not found or could not be updated.\n");
            }

        } else if (choice == 5) {
            printf("\nReturning to Main Menu...\n");
            break;
        } else {
            printf("\nInvalid option! Please choose between 1 and 5.\n");
        }
    }
}

