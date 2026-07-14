#include "functions.h"

int saveProduct(const Product* product) {
    FILE* fp = fopen(PRODUCTS_FILE, "a");
    if (fp == NULL) return 0;
    
    fprintf(fp, "%s,%s,%s,%s,%s,%.2f,%d,%d,%d,%ld,%ld\n",
            product->id, product->name, product->description,
            product->category, product->sellerId, product->price,
            product->stock, product->soldCount, product->isActive,
            product->createdAt, product->updatedAt);
    
    fclose(fp);
    return 1;
}

int loadAllProducts(Product** products, int* count) {
    FILE* fp = fopen(PRODUCTS_FILE, "r");
    if (fp == NULL) {
        *products = NULL;
        *count = 0;
        return 0;
    }
    
    Product temp;
    int total = 0;
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%lf,%d,%d,%d,%ld,%ld\n",
                  temp.id, temp.name, temp.description,
                  temp.category, temp.sellerId, &temp.price,
                  &temp.stock, &temp.soldCount, &temp.isActive,
                  &temp.createdAt, &temp.updatedAt) == 11) {
        total++;
    }
    
    if (total == 0) {
        fclose(fp);
        *products = NULL;
        *count = 0;
        return 0;
    }
    
    *products = (Product*)malloc(total * sizeof(Product));
    if (*products == NULL) {
        fclose(fp);
        return 0;
    }
    
    rewind(fp);
    int i = 0;
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%lf,%d,%d,%d,%ld,%ld\n",
                  (*products)[i].id, (*products)[i].name, (*products)[i].description,
                  (*products)[i].category, (*products)[i].sellerId, &(*products)[i].price,
                  &(*products)[i].stock, &(*products)[i].soldCount, &(*products)[i].isActive,
                  &(*products)[i].createdAt, &(*products)[i].updatedAt) == 11) {
        i++;
    }
    
    fclose(fp);
    *count = total;
    return 1;
}

int updateProductInFile(const Product* product) {
    FILE* fp = fopen(PRODUCTS_FILE, "r");
    if (fp == NULL) return 0;
    
    FILE* temp = fopen("temp_products.txt", "w");
    if (temp == NULL) {
        fclose(fp);
        return 0;
    }
    
    Product current;
    int found = 0;
    
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%lf,%d,%d,%d,%ld,%ld\n",
                  current.id, current.name, current.description,
                  current.category, current.sellerId, &current.price,
                  &current.stock, &current.soldCount, &current.isActive,
                  &current.createdAt, &current.updatedAt) == 11) {
        
        if (strcmp(current.id, product->id) == 0) {
            fprintf(temp, "%s,%s,%s,%s,%s,%.2f,%d,%d,%d,%ld,%ld\n",
                    product->id, product->name, product->description,
                    product->category, product->sellerId, product->price,
                    product->stock, product->soldCount, product->isActive,
                    product->createdAt, product->updatedAt);
            found = 1;
        } else {
            fprintf(temp, "%s,%s,%s,%s,%s,%.2f,%d,%d,%d,%ld,%ld\n",
                    current.id, current.name, current.description,
                    current.category, current.sellerId, current.price,
                    current.stock, current.soldCount, current.isActive,
                    current.createdAt, current.updatedAt);
        }
    }
    
    fclose(fp);
    fclose(temp);
    
    if (found) {
        remove(PRODUCTS_FILE);
        rename("temp_products.txt", PRODUCTS_FILE);
        return 1;
    } else {
        remove("temp_products.txt");
        return 0;
    }
}

int deleteProductFromFile(const char* productId) {
    FILE* fp = fopen(PRODUCTS_FILE, "r");
    if (fp == NULL) return 0;
    
    FILE* temp = fopen("temp_products.txt", "w");
    if (temp == NULL) {
        fclose(fp);
        return 0;
    }
    
    Product current;
    int found = 0;
    
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%lf,%d,%d,%d,%ld,%ld\n",
                  current.id, current.name, current.description,
                  current.category, current.sellerId, &current.price,
                  &current.stock, &current.soldCount, &current.isActive,
                  &current.createdAt, &current.updatedAt) == 11) {
        
        if (strcmp(current.id, productId) == 0) {
            current.isActive = 0;
            fprintf(temp, "%s,%s,%s,%s,%s,%.2f,%d,%d,%d,%ld,%ld\n",
                    current.id, current.name, current.description,
                    current.category, current.sellerId, current.price,
                    current.stock, current.soldCount, current.isActive,
                    current.createdAt, current.updatedAt);
            found = 1;
        } else {
            fprintf(temp, "%s,%s,%s,%s,%s,%.2f,%d,%d,%d,%ld,%ld\n",
                    current.id, current.name, current.description,
                    current.category, current.sellerId, current.price,
                    current.stock, current.soldCount, current.isActive,
                    current.createdAt, current.updatedAt);
        }
    }
    
    fclose(fp);
    fclose(temp);
    
    if (found) {
        remove(PRODUCTS_FILE);
        rename("temp_products.txt", PRODUCTS_FILE);
        return 1;
    } else {
        remove("temp_products.txt");
        return 0;
    }
}

Product* findProductById(const char* productId) {
    FILE* fp = fopen(PRODUCTS_FILE, "r");
    if (fp == NULL) return NULL;
    
    Product product;
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%lf,%d,%d,%d,%ld,%ld\n",
                  product.id, product.name, product.description,
                  product.category, product.sellerId, &product.price,
                  &product.stock, &product.soldCount, &product.isActive,
                  &product.createdAt, &product.updatedAt) == 11) {
        if (strcmp(product.id, productId) == 0) {
            fclose(fp);
            Product* foundProduct = (Product*)malloc(sizeof(Product));
            *foundProduct = product;
            return foundProduct;
        }
    }
    
    fclose(fp);
    return NULL;
}

Product* findProductByName(const char* name) {
    FILE* fp = fopen(PRODUCTS_FILE, "r");
    if (fp == NULL) return NULL;
    
    Product product;
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%lf,%d,%d,%d,%ld,%ld\n",
                  product.id, product.name, product.description,
                  product.category, product.sellerId, &product.price,
                  &product.stock, &product.soldCount, &product.isActive,
                  &product.createdAt, &product.updatedAt) == 11) {
        if (strcmp(product.name, name) == 0 && product.isActive) {
            fclose(fp);
            Product* foundProduct = (Product*)malloc(sizeof(Product));
            *foundProduct = product;
            return foundProduct;
        }
    }
    
    fclose(fp);
    return NULL;
}

int searchProductsByCategory(const char* category, Product** results, int* count) {
    FILE* fp = fopen(PRODUCTS_FILE, "r");
    if (fp == NULL) {
        *results = NULL;
        *count = 0;
        return 0;
    }
    
    Product* tempResults = NULL;
    int tempCount = 0;
    Product current;
    
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%lf,%d,%d,%d,%ld,%ld\n",
                  current.id, current.name, current.description,
                  current.category, current.sellerId, &current.price,
                  &current.stock, &current.soldCount, &current.isActive,
                  &current.createdAt, &current.updatedAt) == 11) {
        
        if (strcmp(current.category, category) == 0 && current.isActive) {
            tempResults = (Product*)realloc(tempResults, (tempCount + 1) * sizeof(Product));
            tempResults[tempCount] = current;
            tempCount++;
        }
    }
    
    fclose(fp);
    
    if (tempCount > 0) {
        *results = tempResults;
        *count = tempCount;
        return 1;
    } else {
        *results = NULL;
        *count = 0;
        return 0;
    }
}

int searchProductsBySeller(const char* sellerId, Product** results, int* count) {
    FILE* fp = fopen(PRODUCTS_FILE, "r");
    if (fp == NULL) {
        *results = NULL;
        *count = 0;
        return 0;
    }
    
    Product* tempResults = NULL;
    int tempCount = 0;
    Product current;
    
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%lf,%d,%d,%d,%ld,%ld\n",
                  current.id, current.name, current.description,
                  current.category, current.sellerId, &current.price,
                  &current.stock, &current.soldCount, &current.isActive,
                  &current.createdAt, &current.updatedAt) == 11) {
        
        if (strcmp(current.sellerId, sellerId) == 0 && current.isActive) {
            tempResults = (Product*)realloc(tempResults, (tempCount + 1) * sizeof(Product));
            tempResults[tempCount] = current;
            tempCount++;
        }
    }
    
    fclose(fp);
    
    if (tempCount > 0) {
        *results = tempResults;
        *count = tempCount;
        return 1;
    } else {
        *results = NULL;
        *count = 0;
        return 0;
    }
}

int validateProduct(const Product* product) {
    if (product == NULL) return 0;
    
    if (strlen(product->name) < 2 || strlen(product->name) > MAX_PRODUCT_NAME) {
        return 0;
    }
    
    if (strlen(product->description) < 5 || strlen(product->description) > MAX_DESCRIPTION) {
        return 0;
    }
    
    if (strlen(product->category) < 2 || strlen(product->category) > MAX_CATEGORY) {
        return 0;
    }
    
    if (strlen(product->sellerId) == 0) {
        return 0;
    }
    
    if (product->price < 0) {
        return 0;
    }
    
    if (product->stock < 0) {
        return 0;
    }
    
    return 1;
}

int updateProductStock(const char* productId, int newStock) {
    Product* product = findProductById(productId);
    if (product == NULL) return 0;
    
    product->stock = newStock;
    product->updatedAt = time(NULL);
    
    int result = updateProductInFile(product);
    free(product);
    return result;
}