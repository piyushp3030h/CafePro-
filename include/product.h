#ifndef PRODUCT_H
#define PRODUCT_H

#define MAX_NAME_LEN 50
#define MAX_CATEGORY_LEN 30
#define PRODUCT_DATA_FILE "data/products.dat"

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char category[MAX_CATEGORY_LEN];
    float price;
    int stock_quantity;
} Product;

void productMenu(void);
int addProduct(const Product *product);
void viewProducts(void);
int findProductById(int id, Product *found);
int updateProductStock(int id, int quantity_change);

#endif
