#include "functions.h"

void sellerDashboard() {
    int choice;
    
    while (1) {
        clearScreen();
        printf("\n");
        centerTitle("SELLER DASHBOARD");
        char welcomeMsg[100];
        sprintf(welcomeMsg, "Welcome, %s (Seller)", currentUser.fullName);
        centerText(welcomeMsg);
        printf("\n");
        
        const char* options[] = {
            "1. Product Management",
            "2. View My Products",
            "3. Update Stock",
            "4. View Order History",
            "5. View Sales Report",
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
                sellerProductManagement();
                break;
            case 2:
                viewSellerProducts();
                break;
            case 3:
                updateStock();
                break;
            case 4:
                viewSellerOrders();
                break;
            case 5:
                viewSellerSales();
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

void sellerProductManagement() {
    int choice;
    
    while (1) {
        clearScreen();
        centerTitle("SELLER PRODUCT MANAGEMENT");
        
        const char* options[] = {
            "1. Add Product",
            "2. Edit Product",
            "3. Delete Product",
            "4. Back to Dashboard"
        };
        
        for (int i = 0; i < 4; i++) {
            centerText(options[i]);
        }
        
        choice = getIntegerInput("Enter your choice: ");
        
        switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                editProduct();
                break;
            case 3:
                deleteProduct();
                break;
            case 4:
                return;
            default:
                centerText("Invalid choice!");
                pauseScreen();
        }
    }
}

void addProduct() {
    clearScreen();
    centerTitle("ADD NEW PRODUCT");
    
    Product newProduct;
    
    printf("\n");
    getStringInput("Product Name: ", newProduct.name, sizeof(newProduct.name));
    getStringInput("Description: ", newProduct.description, sizeof(newProduct.description));
    getStringInput("Category: ", newProduct.category, sizeof(newProduct.category));
    
    newProduct.price = getDoubleInput("Price (BDT): ");
    
    if (newProduct.price <= 0) {
        centerText("Price must be greater than 0!");
        pauseScreen();
        return;
    }
    
    newProduct.stock = getIntegerInput("Initial Stock: ");
    
    if (newProduct.stock < 0) {
        centerText("Stock cannot be negative!");
        pauseScreen();
        return;
    }
    
    strcpy(newProduct.id, generateId("PRD"));
    strcpy(newProduct.sellerId, currentUser.id);
    newProduct.soldCount = 0;
    newProduct.isActive = 1;
    newProduct.createdAt = time(NULL);
    newProduct.updatedAt = time(NULL);
    
    if (!validateProduct(&newProduct)) {
        centerText("Invalid product data!");
        pauseScreen();
        return;
    }
    
    if (saveProduct(&newProduct)) {
        char msg[100];
        sprintf(msg, "Product added successfully! Product ID: %s", newProduct.id);
        centerText(msg);
        centerText("Shipping charge is managed globally by admin.");
    } else {
        centerText("Failed to add product!");
    }
    
    pauseScreen();
}

void editProduct() {
    clearScreen();
    centerTitle("EDIT PRODUCT");
    
    char productId[MAX_ID];
    getStringInput("Enter Product ID to edit: ", productId, sizeof(productId));
    
    Product* product = findProductById(productId);
    if (product == NULL) {
        centerText("Product not found!");
        pauseScreen();
        return;
    }
    
    if (strcmp(product->sellerId, currentUser.id) != 0) {
        centerText("You can only edit your own products!");
        free(product);
        pauseScreen();
        return;
    }
    
    clearScreen();
    char title[100];
    sprintf(title, "EDIT PRODUCT: %s", product->name);
    centerTitle(title);
    
    printf("\n");
    char info[200];
    sprintf(info, "Current Name: %s", product->name);
    centerText(info);
    char newName[MAX_PRODUCT_NAME];
    getStringInput("New Name (press Enter to keep current): ", newName, sizeof(newName));
    if (strlen(newName) > 0) {
        strcpy(product->name, newName);
    }
    
    sprintf(info, "Current Description: %s", product->description);
    centerText(info);
    char newDesc[MAX_DESCRIPTION];
    getStringInput("New Description (press Enter to keep current): ", newDesc, sizeof(newDesc));
    if (strlen(newDesc) > 0) {
        strcpy(product->description, newDesc);
    }
    
    sprintf(info, "Current Category: %s", product->category);
    centerText(info);
    char newCategory[MAX_CATEGORY];
    getStringInput("New Category (press Enter to keep current): ", newCategory, sizeof(newCategory));
    if (strlen(newCategory) > 0) {
        strcpy(product->category, newCategory);
    }
    
    sprintf(info, "Current Price: BDT %.2f", product->price);
    centerText(info);
    double newPrice = getDoubleInput("New Price (enter 0 to keep current): ");
    if (newPrice > 0) {
        product->price = newPrice;
    }
    
    product->updatedAt = time(NULL);
    
    if (updateProductInFile(product)) {
        centerText("Product updated successfully!");
        centerText("Note: Shipping charge is managed globally by admin.");
    } else {
        centerText("Failed to update product!");
    }
    
    free(product);
    pauseScreen();
}

void deleteProduct() {
    clearScreen();
    centerTitle("DELETE PRODUCT");
    
    char productId[MAX_ID];
    getStringInput("Enter Product ID to delete: ", productId, sizeof(productId));
    
    Product* product = findProductById(productId);
    if (product == NULL) {
        centerText("Product not found!");
        pauseScreen();
        return;
    }
    
    if (strcmp(product->sellerId, currentUser.id) != 0) {
        centerText("You can only delete your own products!");
        free(product);
        pauseScreen();
        return;
    }
    
    printf("\n");
    char info[200];
    sprintf(info, "Product Details:");
    centerText(info);
    sprintf(info, "Name: %s", product->name);
    centerText(info);
    sprintf(info, "Price: BDT %.2f", product->price);
    centerText(info);
    sprintf(info, "Stock: %d", product->stock);
    centerText(info);
    
    if (confirmAction("Are you sure you want to delete this product?")) {
        if (deleteProductFromFile(productId)) {
            centerText("Product deleted successfully!");
        } else {
            centerText("Failed to delete product!");
        }
    }
    
    free(product);
    pauseScreen();
}

void viewSellerProducts() {
    clearScreen();
    centerTitle("MY PRODUCTS");
    
    Product* results = NULL;
    int count = 0;
    
    if (searchProductsBySeller(currentUser.id, &results, &count)) {
        printf("\n");
        centerText("ID        Name                      Category      Price      Stock     Sold");
        centerText("------------------------------------------------------------");
        
        for (int i = 0; i < count; i++) {
            if (results[i].isActive) {
                char line[200];
                sprintf(line, "%-10s %-25s %-15s BDT %-9.2f %-10d %-10d", 
                       results[i].id, results[i].name, 
                       results[i].category, results[i].price, 
                       results[i].stock, results[i].soldCount);
                centerText(line);
            }
        }
        free(results);
    } else {
        centerText("You haven't added any products yet.");
    }
    
    pauseScreen();
}

void updateStock() {
    clearScreen();
    centerTitle("UPDATE STOCK");
    
    char productId[MAX_ID];
    getStringInput("Enter Product ID: ", productId, sizeof(productId));
    
    Product* product = findProductById(productId);
    if (product == NULL) {
        centerText("Product not found!");
        pauseScreen();
        return;
    }
    
    if (strcmp(product->sellerId, currentUser.id) != 0) {
        centerText("You can only update stock for your own products!");
        free(product);
        pauseScreen();
        return;
    }
    
    printf("\n");
    char info[200];
    sprintf(info, "Product: %s", product->name);
    centerText(info);
    sprintf(info, "Current Stock: %d", product->stock);
    centerText(info);
    
    int newStock = getIntegerInput("Enter new stock quantity: ");
    
    if (newStock < 0) {
        centerText("Stock cannot be negative!");
        free(product);
        pauseScreen();
        return;
    }
    
    if (updateProductStock(productId, newStock)) {
        sprintf(info, "Stock updated successfully! New stock: %d", newStock);
        centerText(info);
    } else {
        centerText("Failed to update stock!");
    }
    
    free(product);
    pauseScreen();
}

void viewSellerOrders() {
    clearScreen();
    centerTitle("SELLER ORDER HISTORY");
    
    Order* orders = NULL;
    int count = 0;
    
    if (loadOrdersBySeller(currentUser.id, &orders, &count)) {
        printf("\n");
        centerText("Order ID    Buyer         Product               Qty    Total     Status");
        centerText("------------------------------------------------------------");
        
        for (int i = 0; i < count; i++) {
            char line[200];
            sprintf(line, "%-12s %-12s %-20s %-8d BDT %-8.2f %-12s", 
                   orders[i].id, orders[i].buyerId, 
                   orders[i].productName, orders[i].quantity,
                   orders[i].total, orders[i].status);
            centerText(line);
        }
        
        printf("\n");
        centerText("1. Update Order Status");
        centerText("2. View Invoice");
        centerText("3. Back");
        int choice = getIntegerInput("Enter choice: ");
        
        if (choice == 1) {
            char orderId[MAX_ID];
            getStringInput("Enter Order ID to update: ", orderId, sizeof(orderId));
            
            Order* order = findOrderById(orderId);
            if (order == NULL) {
                centerText("Order not found!");
                free(orders);
                pauseScreen();
                return;
            }
            
            if (strcmp(order->sellerId, currentUser.id) != 0) {
                centerText("You can only update status for orders of your products!");
                free(order);
                free(orders);
                pauseScreen();
                return;
            }
            
            printf("\n");
            char msg[100];
            sprintf(msg, "Current Status: %s", order->status);
            centerText(msg);
            centerText("Select new status:");
            centerText("1. Pending");
            centerText("2. Shipped");
            centerText("3. Delivered");
            centerText("4. Cancelled");
            int statusChoice = getIntegerInput("Enter choice: ");
            
            char newStatus[20];
            switch(statusChoice) {
                case 1: strcpy(newStatus, "Pending"); break;
                case 2: strcpy(newStatus, "Shipped"); break;
                case 3: strcpy(newStatus, "Delivered"); break;
                case 4: strcpy(newStatus, "Cancelled"); break;
                default:
                    centerText("Invalid choice!");
                    free(order);
                    free(orders);
                    pauseScreen();
                    return;
            }
            
            if (updateOrderStatus(orderId, newStatus)) {
                sprintf(msg, "Order status updated successfully to: %s", newStatus);
                centerText(msg);
            } else {
                centerText("Failed to update order status!");
            }
            free(order);
        } else if (choice == 2) {
            char orderId[MAX_ID];
            getStringInput("Enter Order ID: ", orderId, sizeof(orderId));
            
            Order* order = findOrderById(orderId);
            if (order != NULL) {
                generateInvoice(order);
                free(order);
            } else {
                centerText("Order not found!");
            }
            pauseScreen();
        }
        free(orders);
    } else {
        centerText("No orders for your products yet.");
        pauseScreen();
    }
}

void viewSellerSales() {
    clearScreen();
    centerTitle("SELLER SALES REPORT");
    
    Product* products = NULL;
    int count = 0;
    
    if (searchProductsBySeller(currentUser.id, &products, &count)) {
        printf("\n");
        centerText("Product Name               Units Sold    Revenue");
        centerText("------------------------------------------------------------");
        
        double totalRevenue = 0;
        int totalSold = 0;
        
        for (int i = 0; i < count; i++) {
            if (products[i].soldCount > 0) {
                double revenue = products[i].soldCount * products[i].price;
                char line[200];
                sprintf(line, "%-25s %-15d BDT %-14.2f", 
                       products[i].name, 
                       products[i].soldCount,
                       revenue);
                centerText(line);
                totalRevenue += revenue;
                totalSold += products[i].soldCount;
            }
        }
        
        centerText("------------------------------------------------------------");
        char summary[200];
        sprintf(summary, "Total Units Sold: %d", totalSold);
        centerText(summary);
        sprintf(summary, "Total Revenue: BDT %.2f", totalRevenue);
        centerText(summary);
        free(products);
    } else {
        centerText("No sales data available.");
    }
    
    pauseScreen();
}