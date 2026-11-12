#include <stdio.h>
#include <string.h>
#include "product.h"
#include "utils.h"

#define PRODUCT_TEMP_FILE "data/products.tmp"

int addProduct(const Product *product) {
    if (product == NULL || product->id <= 0 || product->price < 0 || product->stock_quantity < 0) {
        return 0;
    }
    if (findProductById(product->id, NULL)) {
        return 0;
    }

    FILE *fp = fopen(PRODUCT_DATA_FILE, "ab");
    if (fp == NULL) {
        printf("\n[Error] Could not open %s.\n", PRODUCT_DATA_FILE);
        return 0;
    }

    int written = (int)fwrite(product, sizeof(Product), 1, fp);
    fclose(fp);
    return (written == 1) ? 1 : 0;
}

void viewProducts(void) {
    FILE *fp = fopen(PRODUCT_DATA_FILE, "rb");
    if (fp == NULL) {
        printf("\n[Notice] No products in inventory yet.\n");
        return;
    }

    Product p;
    printf("\n====================================================================\n");
    printf("                       PRODUCT INVENTORY                            \n");
    printf("====================================================================\n");
    printf("%-6s | %-22s | %-15s | %-8s | %-6s\n", "ID", "Name", "Category", "Price", "Stock");
    printf("-------+------------------------+-----------------+----------+------\n");

    while (fread(&p, sizeof(Product), 1, fp) == 1) {
        printf("%-6d | %-22s | %-15s | %8.2f | %-6d\n",
               p.id, p.name, p.category, p.price, p.stock_quantity);
    }
    printf("====================================================================\n");
    fclose(fp);
}

int findProductById(int id, Product *found) {
    FILE *fp = fopen(PRODUCT_DATA_FILE, "rb");
    if (fp == NULL) {
        return 0;
    }

    Product temp;
    while (fread(&temp, sizeof(Product), 1, fp) == 1) {
        if (temp.id == id) {
            if (found != NULL) {
                *found = temp;
            }
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

int updateProductStock(int id, int quantity_change) {
    FILE *fp = fopen(PRODUCT_DATA_FILE, "rb");
    if (fp == NULL) {
        return 0;
    }

    FILE *tmp = fopen(PRODUCT_TEMP_FILE, "wb");
    if (tmp == NULL) {
        fclose(fp);
        return 0;
    }

    Product temp;
    int updated = 0;

    while (fread(&temp, sizeof(Product), 1, fp) == 1) {
        if (temp.id == id) {
            if (temp.stock_quantity + quantity_change < 0) {
                printf("\n[Error] Not enough stock! Available: %d\n", temp.stock_quantity);
                fclose(fp);
                fclose(tmp);
                remove(PRODUCT_TEMP_FILE);
                return 0;
            }
            temp.stock_quantity += quantity_change;
            updated = 1;
        }
        fwrite(&temp, sizeof(Product), 1, tmp);
    }

    fclose(fp);
    fclose(tmp);

    if (updated) {
        remove(PRODUCT_DATA_FILE);
        rename(PRODUCT_TEMP_FILE, PRODUCT_DATA_FILE);
    } else {
        remove(PRODUCT_TEMP_FILE);
    }
    return updated;
}

void productMenu(void) {
    int choice = 0;

    while (!feof(stdin)) {
        printf("\n=== PRODUCT & INVENTORY ===\n");
        printf("1. Add Product\n");
        printf("2. View All Products\n");
        printf("3. Restock Product\n");
        printf("4. Back to Main Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        if (choice == 1) {
            Product p;
            printf("Enter Product ID: ");
            if (scanf("%d", &p.id) != 1) {
                clearInputBuffer();
                continue;
            }
            clearInputBuffer();

            if (findProductById(p.id, NULL)) {
                printf("[Error] Product ID %d already exists!\n", p.id);
                continue;
            }

            printf("Enter Product Name: ");
            readString(p.name, MAX_NAME_LEN);
            printf("Enter Category: ");
            readString(p.category, MAX_CATEGORY_LEN);

            printf("Enter Price: ");
            if (scanf("%f", &p.price) != 1) {
                clearInputBuffer();
                continue;
            }
            printf("Enter Stock Quantity: ");
            if (scanf("%d", &p.stock_quantity) != 1) {
                clearInputBuffer();
                continue;
            }
            clearInputBuffer();

            if (addProduct(&p)) {
                printf("[Success] Product '%s' added!\n", p.name);
            } else {
                printf("[Error] Could not add product.\n");
            }
        } else if (choice == 2) {
            viewProducts();
        } else if (choice == 3) {
            int id, qty;
            printf("Enter Product ID: ");
            if (scanf("%d", &id) != 1) {
                clearInputBuffer();
                continue;
            }
            printf("Enter Quantity to Add: ");
            if (scanf("%d", &qty) != 1 || qty <= 0) {
                clearInputBuffer();
                printf("[Error] Quantity must be positive.\n");
                continue;
            }
            clearInputBuffer();

            if (updateProductStock(id, qty)) {
                printf("[Success] Stock updated!\n");
            } else {
                printf("[Error] Product ID not found.\n");
            }
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice! Pick 1 to 4.\n");
        }
    }
}
