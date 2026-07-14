#include "functions.h"

void buyerDashboard() {
    int choice;
    
    while (1) {
        clearScreen();
        printf("\n");
        centerTitle("BUYER DASHBOARD");
        char welcomeMsg[100];
        sprintf(welcomeMsg, "Welcome, %s (Buyer)", currentUser.fullName);
        centerText(welcomeMsg);
        printf("\n");
        
        const char* options[] = {
            "1. Browse Products",
            "2. Search Products",
            "3. Sort Products",
            "4. View Cart",
            "5. View Order History",
            "6. Change Password",
            "7. Logout"
        };
        
        for (int i = 0; i < 7; i++) {
            centerText(options[i]);
        }
        
        centerText("Enter your choice: ");
        printf("\n\t\t\t\t\t");
        scanf("%d", &choice);
        getchar();
        
        switch (choice) {
            case 1:
                browseProducts();
                break;
            case 2:
                searchProducts();
                break;
            case 3:
                sortProducts();
                break;
            case 4:
                viewCart(currentUser.id);
                break;
            case 5:
                viewOrderHistory(currentUser.id, ROLE_BUYER);
                break;
            case 6:
                changePassword();
                break;
            case 7:
                logout();
                return;
            default:
                centerText("Invalid choice!");
                pauseScreen();
        }
    }
}

void browseProducts() {
    clearScreen();
    centerTitle("BROWSE PRODUCTS");
    
    Product* products = NULL;
    int count = 0;
    
    if (loadAllProducts(&products, &count)) {
        int hasProducts = 0;
        printf("\n");
        centerText("ID        Name                      Category      Price      Stock");
        centerText("------------------------------------------------------------");
        
        for (int i = 0; i < count; i++) {
            if (products[i].isActive && products[i].stock > 0) {
                hasProducts = 1;
                char line[200];
                sprintf(line, "%-10s %-25s %-15s BDT %-9.2f %-10d", 
                       products[i].id, products[i].name, 
                       products[i].category, products[i].price, products[i].stock);
                centerText(line);
            }
        }
        
        if (!hasProducts) {
            centerText("No products available.");
            free(products);
            pauseScreen();
            return;
        }
        
        printf("\n");
        centerText("1. View Product Details");
        centerText("2. Add to Cart");
        centerText("3. Back to Dashboard");
        int choice = getIntegerInput("Enter your choice: ");
        
        if (choice == 1 || choice == 2) {
            char productId[MAX_ID];
            getStringInput("Enter Product ID: ", productId, sizeof(productId));
            
            Product* product = findProductById(productId);
            if (product == NULL) {
                centerText("Product not found!");
                free(products);
                pauseScreen();
                return;
            }
            
            if (choice == 1) {
                viewProductDetails(productId);
            } else {
                if (!product->isActive || product->stock <= 0) {
                    centerText("Product is not available!");
                    free(product);
                    free(products);
                    pauseScreen();
                    return;
                }
                
                int quantity = getIntegerInput("Enter quantity: ");
                
                if (quantity <= 0 || quantity > product->stock) {
                    centerText("Invalid quantity or insufficient stock!");
                } else {
                    if (addToCart(productId, currentUser.id, quantity)) {
                        centerText("Product added to cart successfully!");
                    } else {
                        centerText("Failed to add product to cart!");
                    }
                }
                free(product);
            }
        }
        free(products);
    } else {
        centerText("No products available.");
    }
    
    pauseScreen();
}

void searchProducts() {
    clearScreen();
    centerTitle("SEARCH PRODUCTS");
    
    printf("\n");
    centerText("Search by:");
    centerText("1. Product Name");
    centerText("2. Category");
    centerText("3. Price Range");
    int choice = getIntegerInput("Enter choice: ");
    
    Product* products = NULL;
    int count = 0;
    int found = 0;
    
    switch (choice) {
        case 1: {
            char name[MAX_PRODUCT_NAME];
            getStringInput("Enter product name: ", name, sizeof(name));
            
            if (loadAllProducts(&products, &count)) {
                printf("\n");
                centerTitle("SEARCH RESULTS");
                centerText("ID        Name                      Category      Price");
                centerText("------------------------------------------------------------");
                
                for (int i = 0; i < count; i++) {
                    if (products[i].isActive && 
                        strstr(products[i].name, name) != NULL) {
                        found = 1;
                        char line[200];
                        sprintf(line, "%-10s %-25s %-15s BDT %-9.2f", 
                               products[i].id, products[i].name, 
                               products[i].category, products[i].price);
                        centerText(line);
                    }
                }
            }
            break;
        }
        case 2: {
            char category[MAX_CATEGORY];
            getStringInput("Enter category: ", category, sizeof(category));
            
            Product* results = NULL;
            if (searchProductsByCategory(category, &results, &count)) {
                found = 1;
                printf("\n");
                char title[100];
                sprintf(title, "PRODUCTS IN CATEGORY: %s", category);
                centerTitle(title);
                centerText("ID        Name                      Price");
                centerText("------------------------------------------------------------");
                
                for (int i = 0; i < count; i++) {
                    char line[200];
                    sprintf(line, "%-10s %-25s BDT %-9.2f", 
                           results[i].id, results[i].name, 
                           results[i].price);
                    centerText(line);
                }
                free(results);
            }
            break;
        }
        case 3: {
            double minPrice = getDoubleInput("Enter minimum price (BDT): ");
            double maxPrice = getDoubleInput("Enter maximum price (BDT): ");
            
            if (loadAllProducts(&products, &count)) {
                printf("\n");
                centerTitle("PRODUCTS IN PRICE RANGE");
                centerText("ID        Name                      Category      Price");
                centerText("------------------------------------------------------------");
                
                for (int i = 0; i < count; i++) {
                    if (products[i].isActive && 
                        products[i].price >= minPrice && 
                        products[i].price <= maxPrice) {
                        found = 1;
                        char line[200];
                        sprintf(line, "%-10s %-25s %-15s BDT %-9.2f", 
                               products[i].id, products[i].name, 
                               products[i].category, products[i].price);
                        centerText(line);
                    }
                }
            }
            break;
        }
        default:
            centerText("Invalid choice!");
            pauseScreen();
            return;
    }
    
    if (!found) {
        printf("\n");
        centerText("No products found matching your search.");
    }
    
    if (products) free(products);
    pauseScreen();
}

void sortProducts() {
    clearScreen();
    centerTitle("SORT PRODUCTS");
    
    printf("\n");
    centerText("Sort by:");
    centerText("1. Price (Low to High)");
    centerText("2. Price (High to Low)");
    centerText("3. Name (A to Z)");
    centerText("4. Name (Z to A)");
    centerText("5. Stock (High to Low)");
    int choice = getIntegerInput("Enter choice: ");
    
    Product* products = NULL;
    int count = 0;
    
    if (!loadAllProducts(&products, &count)) {
        centerText("No products available.");
        pauseScreen();
        return;
    }
    
    Product* activeProducts = NULL;
    int activeCount = 0;
    for (int i = 0; i < count; i++) {
        if (products[i].isActive) {
            activeProducts = (Product*)realloc(activeProducts, (activeCount + 1) * sizeof(Product));
            activeProducts[activeCount] = products[i];
            activeCount++;
        }
    }
    
    if (activeCount == 0) {
        centerText("No active products available.");
        free(products);
        pauseScreen();
        return;
    }
    
    for (int i = 0; i < activeCount - 1; i++) {
        for (int j = 0; j < activeCount - i - 1; j++) {
            int swap = 0;
            switch (choice) {
                case 1:
                    if (activeProducts[j].price > activeProducts[j + 1].price) swap = 1;
                    break;
                case 2:
                    if (activeProducts[j].price < activeProducts[j + 1].price) swap = 1;
                    break;
                case 3:
                    if (strcmp(activeProducts[j].name, activeProducts[j + 1].name) > 0) swap = 1;
                    break;
                case 4:
                    if (strcmp(activeProducts[j].name, activeProducts[j + 1].name) < 0) swap = 1;
                    break;
                case 5:
                    if (activeProducts[j].stock < activeProducts[j + 1].stock) swap = 1;
                    break;
                default:
                    centerText("Invalid choice!");
                    free(products);
                    free(activeProducts);
                    pauseScreen();
                    return;
            }
            if (swap) {
                Product temp = activeProducts[j];
                activeProducts[j] = activeProducts[j + 1];
                activeProducts[j + 1] = temp;
            }
        }
    }
    
    printf("\n");
    centerTitle("SORTED PRODUCTS");
    centerText("ID        Name                      Category      Price      Stock");
    centerText("------------------------------------------------------------");
    
    for (int i = 0; i < activeCount; i++) {
        char line[200];
        sprintf(line, "%-10s %-25s %-15s BDT %-9.2f %-10d", 
               activeProducts[i].id, activeProducts[i].name, 
               activeProducts[i].category, activeProducts[i].price, 
               activeProducts[i].stock);
        centerText(line);
    }
    
    printf("\n");
    centerText("Enter Product ID to add to cart (or 0 to skip): ");
    char productId[MAX_ID];
    fgets(productId, sizeof(productId), stdin);
    trimString(productId);
    
    if (strcmp(productId, "0") != 0) {
        Product* product = findProductById(productId);
        if (product != NULL && product->isActive && product->stock > 0) {
            int quantity = getIntegerInput("Enter quantity: ");
            
            if (quantity > 0 && quantity <= product->stock) {
                if (addToCart(productId, currentUser.id, quantity)) {
                    centerText("Product added to cart successfully!");
                } else {
                    centerText("Failed to add product to cart!");
                }
            } else {
                centerText("Invalid quantity or insufficient stock!");
            }
            free(product);
        } else {
            centerText("Product not available!");
        }
    }
    
    free(products);
    free(activeProducts);
    pauseScreen();
}

void viewProductDetails(const char* productId) {
    Product* product = findProductById(productId);
    if (product == NULL) {
        centerText("Product not found!");
        return;
    }
    
    clearScreen();
    centerTitle("PRODUCT DETAILS");
    
    printf("\n");
    char info[200];
    sprintf(info, "ID: %s", product->id);
    centerText(info);
    sprintf(info, "Name: %s", product->name);
    centerText(info);
    sprintf(info, "Description: %s", product->description);
    centerText(info);
    sprintf(info, "Category: %s", product->category);
    centerText(info);
    sprintf(info, "Seller ID: %s", product->sellerId);
    centerText(info);
    sprintf(info, "Price: BDT %.2f", product->price);
    centerText(info);
    sprintf(info, "Stock Available: %d", product->stock);
    centerText(info);
    sprintf(info, "Total Sold: %d", product->soldCount);
    centerText(info);
    sprintf(info, "Shipping Charge: BDT %.2f (Flat per order)", globalShippingCharge);
    centerText(info);
    sprintf(info, "Added: %s", ctime(&product->createdAt));
    centerText(info);
    sprintf(info, "Last Updated: %s", ctime(&product->updatedAt));
    centerText(info);
    
    if (product->isActive && product->stock > 0) {
        printf("\n");
        centerText("1. Add to Cart");
        centerText("2. Back");
        int choice = getIntegerInput("Enter choice: ");
        
        if (choice == 1) {
            int quantity = getIntegerInput("Enter quantity: ");
            
            if (quantity > 0 && quantity <= product->stock) {
                if (addToCart(productId, currentUser.id, quantity)) {
                    centerText("Product added to cart successfully!");
                } else {
                    centerText("Failed to add product to cart!");
                }
            } else {
                centerText("Invalid quantity or insufficient stock!");
            }
        }
    } else {
        printf("\n");
        centerText("This product is currently unavailable.");
        pauseScreen();
    }
    
    free(product);
    pauseScreen();
}