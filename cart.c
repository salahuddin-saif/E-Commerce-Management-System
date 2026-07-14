#include "functions.h"

int addToCart(const char* productId, const char* buyerId, int quantity) {
    if (productId == NULL || buyerId == NULL || quantity <= 0) {
        return 0;
    }
    
    Product* product = findProductById(productId);
    if (product == NULL || !product->isActive || product->stock < quantity) {
        if (product) free(product);
        return 0;
    }
    free(product);
    
    CartItem* items = NULL;
    int count = 0;
    if (loadCartItems(buyerId, &items, &count)) {
        for (int i = 0; i < count; i++) {
            if (strcmp(items[i].productId, productId) == 0) {
                int newQuantity = items[i].quantity + quantity;
                free(items);
                return updateCartQuantity(productId, buyerId, newQuantity);
            }
        }
        free(items);
    }
    
    CartItem newItem;
    strcpy(newItem.productId, productId);
    strcpy(newItem.buyerId, buyerId);
    newItem.quantity = quantity;
    newItem.addedAt = time(NULL);
    
    return saveCartItem(&newItem);
}

int removeFromCart(const char* productId, const char* buyerId) {
    if (productId == NULL || buyerId == NULL) {
        return 0;
    }
    
    return removeCartItemFromFile(productId, buyerId);
}

int updateCartQuantity(const char* productId, const char* buyerId, int newQuantity) {
    if (productId == NULL || buyerId == NULL || newQuantity <= 0) {
        return 0;
    }
    
    Product* product = findProductById(productId);
    if (product == NULL || !product->isActive || product->stock < newQuantity) {
        if (product) free(product);
        return 0;
    }
    free(product);
    
    FILE* fp = fopen(CART_FILE, "r");
    if (fp == NULL) return 0;
    
    FILE* temp = fopen("temp_cart.txt", "w");
    if (temp == NULL) {
        fclose(fp);
        return 0;
    }
    
    CartItem current;
    int found = 0;
    
    while (fscanf(fp, "%[^,],%[^,],%d,%ld\n",
                  current.productId, current.buyerId,
                  &current.quantity, &current.addedAt) == 4) {
        
        if (strcmp(current.productId, productId) == 0 && 
            strcmp(current.buyerId, buyerId) == 0) {
            current.quantity = newQuantity;
            fprintf(temp, "%s,%s,%d,%ld\n",
                    current.productId, current.buyerId,
                    current.quantity, current.addedAt);
            found = 1;
        } else {
            fprintf(temp, "%s,%s,%d,%ld\n",
                    current.productId, current.buyerId,
                    current.quantity, current.addedAt);
        }
    }
    
    fclose(fp);
    fclose(temp);
    
    if (found) {
        remove(CART_FILE);
        rename("temp_cart.txt", CART_FILE);
        return 1;
    } else {
        remove("temp_cart.txt");
        return 0;
    }
}

int viewCart(const char* buyerId) {
    if (buyerId == NULL) {
        return 0;
    }
    
    clearScreen();
    centerTitle("SHOPPING CART");
    
    CartItem* items = NULL;
    int count = 0;
    
    if (!loadCartItems(buyerId, &items, &count)) {
        centerText("Your cart is empty.");
        pauseScreen();
        return 0;
    }
    
    printf("\n");
    centerText("Product Name             Price      Qty    Subtotal");
    centerText("------------------------------------------------------------");
    
    double total = 0;
    int hasValidItems = 0;
    
    for (int i = 0; i < count; i++) {
        Product* product = findProductById(items[i].productId);
        if (product != NULL && product->isActive) {
            double subtotal = product->price * items[i].quantity;
            char line[200];
            sprintf(line, "%-25s BDT %-8.2f %-8d BDT %-9.2f", 
                   product->name, product->price, 
                   items[i].quantity, subtotal);
            centerText(line);
            total += subtotal;
            hasValidItems = 1;
            free(product);
        } else {
            if (product) free(product);
            removeFromCart(items[i].productId, buyerId);
        }
    }
    
    if (!hasValidItems) {
        centerText("No valid items in cart.");
        free(items);
        pauseScreen();
        return 0;
    }
    
    centerText("------------------------------------------------------------");
    char totalLine[200];
    sprintf(totalLine, "Subtotal: BDT %.2f", total);
    centerText(totalLine);
    sprintf(totalLine, "Shipping Charge (Flat per order): BDT %.2f", globalShippingCharge);
    centerText(totalLine);
    sprintf(totalLine, "Total: BDT %.2f", total + globalShippingCharge);
    centerText(totalLine);
    centerText("================================================");
    
    printf("\n");
    centerText("1. Checkout");
    centerText("2. Remove Item");
    centerText("3. Update Quantity");
    centerText("4. Clear Cart");
    centerText("5. Back to Dashboard");
    int choice = getIntegerInput("Enter choice: ");
    
    switch (choice) {
        case 1:
            if (checkout(buyerId)) {
                centerText("Order placed successfully!");
            } else {
                centerText("Failed to place order.");
            }
            break;
        case 2: {
            char productId[MAX_ID];
            getStringInput("Enter Product ID to remove: ", productId, sizeof(productId));
            if (removeFromCart(productId, buyerId)) {
                centerText("Item removed from cart.");
            } else {
                centerText("Failed to remove item.");
            }
            break;
        }
        case 3: {
            char productId[MAX_ID];
            getStringInput("Enter Product ID: ", productId, sizeof(productId));
            int newQty = getIntegerInput("Enter new quantity: ");
            if (updateCartQuantity(productId, buyerId, newQty)) {
                centerText("Quantity updated.");
            } else {
                centerText("Failed to update quantity.");
            }
            break;
        }
        case 4:
            if (confirmAction("Clear entire cart?")) {
                if (clearCart(buyerId)) {
                    centerText("Cart cleared.");
                } else {
                    centerText("Failed to clear cart.");
                }
            }
            break;
        case 5:
            break;
        default:
            centerText("Invalid choice!");
    }
    
    free(items);
    pauseScreen();
    return 1;
}

int clearCart(const char* buyerId) {
    if (buyerId == NULL) return 0;
    
    FILE* fp = fopen(CART_FILE, "r");
    if (fp == NULL) return 0;
    
    FILE* temp = fopen("temp_cart.txt", "w");
    if (temp == NULL) {
        fclose(fp);
        return 0;
    }
    
    CartItem current;
    int found = 0;
    
    while (fscanf(fp, "%[^,],%[^,],%d,%ld\n",
                  current.productId, current.buyerId,
                  &current.quantity, &current.addedAt) == 4) {
        
        if (strcmp(current.buyerId, buyerId) != 0) {
            fprintf(temp, "%s,%s,%d,%ld\n",
                    current.productId, current.buyerId,
                    current.quantity, current.addedAt);
        } else {
            found = 1;
        }
    }
    
    fclose(fp);
    fclose(temp);
    
    if (found) {
        remove(CART_FILE);
        rename("temp_cart.txt", CART_FILE);
        return 1;
    } else {
        remove("temp_cart.txt");
        return 0;
    }
}

int checkout(const char* buyerId) {
    if (buyerId == NULL) return 0;
    
    CartItem* items = NULL;
    int count = 0;
    
    if (!loadCartItems(buyerId, &items, &count)) {
        centerText("Your cart is empty.");
        return 0;
    }
    
    double total = 0;
    int orderCount = 0;
    
    for (int i = 0; i < count; i++) {
        Product* product = findProductById(items[i].productId);
        if (product == NULL || !product->isActive || product->stock < items[i].quantity) {
            if (product) free(product);
            char msg[100];
            sprintf(msg, "Product %s is not available in requested quantity.", items[i].productId);
            centerText(msg);
            continue;
        }
        
        Order newOrder;
        strcpy(newOrder.id, generateId("ORD"));
        strcpy(newOrder.buyerId, buyerId);
        strcpy(newOrder.sellerId, product->sellerId);
        strcpy(newOrder.productId, product->id);
        strcpy(newOrder.productName, product->name);
        newOrder.quantity = items[i].quantity;
        newOrder.price = product->price;
        
        double shippingToAdd = 0;
        if (i == 0) {
            shippingToAdd = globalShippingCharge;
        }
        
        newOrder.total = (product->price * items[i].quantity) + shippingToAdd;
        strcpy(newOrder.status, "Pending");
        newOrder.orderDate = time(NULL);
        strcpy(newOrder.shippingAddress, currentUser.address);
        newOrder.shippingCharge = shippingToAdd;
        
        if (saveOrder(&newOrder)) {
            product->stock -= items[i].quantity;
            product->soldCount += items[i].quantity;
            product->updatedAt = time(NULL);
            updateProductInFile(product);
            
            total += newOrder.total;
            orderCount++;
        }
        
        free(product);
    }
    
    clearCart(buyerId);
    free(items);
    
    if (orderCount > 0) {
        printf("\n");
        char msg[100];
        sprintf(msg, "Order placed successfully!");
        centerText(msg);
        sprintf(msg, "Total items: %d", orderCount);
        centerText(msg);
        sprintf(msg, "Shipping Charge (Flat per order): BDT %.2f", globalShippingCharge);
        centerText(msg);
        sprintf(msg, "Total amount: BDT %.2f", total);
        centerText(msg);
        centerText("Check your order history for details.");
        return 1;
    } else {
        centerText("No items could be ordered.");
        return 0;
    }
}

int loadCartItems(const char* buyerId, CartItem** items, int* count) {
    if (buyerId == NULL) {
        *items = NULL;
        *count = 0;
        return 0;
    }
    
    FILE* fp = fopen(CART_FILE, "r");
    if (fp == NULL) {
        *items = NULL;
        *count = 0;
        return 0;
    }
    
    CartItem* tempItems = NULL;
    int tempCount = 0;
    CartItem current;
    
    while (fscanf(fp, "%[^,],%[^,],%d,%ld\n",
                  current.productId, current.buyerId,
                  &current.quantity, &current.addedAt) == 4) {
        
        if (strcmp(current.buyerId, buyerId) == 0) {
            tempItems = (CartItem*)realloc(tempItems, (tempCount + 1) * sizeof(CartItem));
            tempItems[tempCount] = current;
            tempCount++;
        }
    }
    
    fclose(fp);
    
    if (tempCount > 0) {
        *items = tempItems;
        *count = tempCount;
        return 1;
    } else {
        *items = NULL;
        *count = 0;
        return 0;
    }
}

int saveCartItem(const CartItem* item) {
    if (item == NULL) return 0;
    
    FILE* fp = fopen(CART_FILE, "a");
    if (fp == NULL) return 0;
    
    fprintf(fp, "%s,%s,%d,%ld\n",
            item->productId, item->buyerId,
            item->quantity, item->addedAt);
    
    fclose(fp);
    return 1;
}

int removeCartItemFromFile(const char* productId, const char* buyerId) {
    if (productId == NULL || buyerId == NULL) return 0;
    
    FILE* fp = fopen(CART_FILE, "r");
    if (fp == NULL) return 0;
    
    FILE* temp = fopen("temp_cart.txt", "w");
    if (temp == NULL) {
        fclose(fp);
        return 0;
    }
    
    CartItem current;
    int found = 0;
    
    while (fscanf(fp, "%[^,],%[^,],%d,%ld\n",
                  current.productId, current.buyerId,
                  &current.quantity, &current.addedAt) == 4) {
        
        if (strcmp(current.productId, productId) == 0 && 
            strcmp(current.buyerId, buyerId) == 0) {
            found = 1;
            continue;
        }
        
        fprintf(temp, "%s,%s,%d,%ld\n",
                current.productId, current.buyerId,
                current.quantity, current.addedAt);
    }
    
    fclose(fp);
    fclose(temp);
    
    if (found) {
        remove(CART_FILE);
        rename("temp_cart.txt", CART_FILE);
        return 1;
    } else {
        remove("temp_cart.txt");
        return 0;
    }
}