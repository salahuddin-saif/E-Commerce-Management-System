#include "functions.h"

void adminDashboard() {
    int choice;
    
    while (1) {
        clearScreen();
        printf("\n");
        centerTitle("ADMIN DASHBOARD");
        char welcomeMsg[100];
        sprintf(welcomeMsg, "Welcome, %s (Administrator)", currentUser.fullName);
        centerText(welcomeMsg);
        printf("\n");
        
        const char* options[] = {
            "1. Approve Sellers",
            "2. Manage Users",
            "3. Product Management",
            "4. View Sales Report",
            "5. View All Orders",
            "6. Manage Shipping Charge",
            "7. Change Password",
            "8. Logout"
        };
        
        for (int i = 0; i < 8; i++) {
            centerText(options[i]);
        }
        
        centerText("Enter your choice: ");
        printf("\n\t\t\t\t\t");
        scanf("%d", &choice);
        getchar();
        
        switch (choice) {
            case 1:
                approveSellers();
                break;
            case 2:
                manageUsers();
                break;
            case 3:
                adminProductManagement();
                break;
            case 4:
                viewSalesReport();
                break;
            case 5:
                viewAllOrders();
                break;
            case 6:
                manageShippingCharge();
                break;
            case 7:
                changePassword();
                break;
            case 8:
                logout();
                return;
            default:
                centerText("Invalid choice!");
                pauseScreen();
        }
    }
}

void approveSellers() {
    clearScreen();
    centerTitle("APPROVE SELLERS");
    
    User* users = NULL;
    int count = 0;
    
    if (!loadAllUsers(&users, &count)) {
        centerText("No users found!");
        pauseScreen();
        return;
    }
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (users[i].role == ROLE_SELLER && users[i].status == STATUS_PENDING) {
            found = 1;
            printf("\n");
            centerText("----------------------------------------");
            char info[200];
            sprintf(info, "Seller ID: %s", users[i].id);
            centerText(info);
            sprintf(info, "Username: %s", users[i].username);
            centerText(info);
            sprintf(info, "Full Name: %s", users[i].fullName);
            centerText(info);
            sprintf(info, "Email: %s", users[i].email);
            centerText(info);
            sprintf(info, "Phone: %s", users[i].phone);
            centerText(info);
            sprintf(info, "Address: %s", users[i].address);
            centerText(info);
            centerText("----------------------------------------");
            
            centerText("1. Approve");
            centerText("2. Reject");
            centerText("3. Skip");
            int action = getIntegerInput("Enter action: ");
            
            if (action == 1) {
                users[i].status = STATUS_APPROVED;
                if (updateUserInFile(&users[i])) {
                    centerText("Seller approved successfully!");
                }
            } else if (action == 2) {
                users[i].status = STATUS_REJECTED;
                if (updateUserInFile(&users[i])) {
                    centerText("Seller rejected!");
                }
            }
        }
    }
    
    if (!found) {
        centerText("No pending seller requests.");
    }
    
    free(users);
    pauseScreen();
}

void manageUsers() {
    int choice;
    
    while (1) {
        clearScreen();
        centerTitle("MANAGE USERS");
        
        const char* options[] = {
            "1. View All Users",
            "2. Activate/Deactivate User",
            "3. Delete User",
            "4. Search User",
            "5. Back to Dashboard"
        };
        
        for (int i = 0; i < 5; i++) {
            centerText(options[i]);
        }
        
        choice = getIntegerInput("Enter your choice: ");
        
        switch (choice) {
            case 1: {
                User* users = NULL;
                int count = 0;
                if (loadAllUsers(&users, &count)) {
                    clearScreen();
                    centerTitle("ALL USERS");
                    printf("\n");
                    centerText("ID        Username        Full Name           Role      Status");
                    centerText("------------------------------------------------------------");
                    
                    for (int i = 0; i < count; i++) {
                        char* roleStr = (users[i].role == ROLE_ADMIN) ? "Admin" :
                                       (users[i].role == ROLE_SELLER) ? "Seller" : "Buyer";
                        char* statusStr = (users[i].status == STATUS_APPROVED) ? "Approved" :
                                         (users[i].status == STATUS_PENDING) ? "Pending" : "Rejected";
                        char line[200];
                        sprintf(line, "%-10s %-15s %-20s %-10s %-10s", 
                               users[i].id, users[i].username, 
                               users[i].fullName, roleStr, statusStr);
                        centerText(line);
                    }
                    free(users);
                } else {
                    centerText("No users found!");
                }
                pauseScreen();
                break;
            }
            case 2: {
                char username[MAX_USERNAME];
                getStringInput("Enter username to activate/deactivate: ", username, sizeof(username));
                
                User* user = findUserByUsername(username);
                if (user == NULL) {
                    centerText("User not found!");
                    pauseScreen();
                    break;
                }
                
                if (strcmp(user->username, currentUser.username) == 0) {
                    centerText("You cannot deactivate your own account!");
                    free(user);
                    pauseScreen();
                    break;
                }
                
                user->isActive = !user->isActive;
                if (updateUserInFile(user)) {
                    char msg[100];
                    sprintf(msg, "User %s successfully!", user->isActive ? "activated" : "deactivated");
                    centerText(msg);
                } else {
                    centerText("Failed to update user!");
                }
                free(user);
                pauseScreen();
                break;
            }
            case 3: {
                char username[MAX_USERNAME];
                getStringInput("Enter username to delete: ", username, sizeof(username));
                
                if (strcmp(username, currentUser.username) == 0) {
                    centerText("You cannot delete your own account!");
                    pauseScreen();
                    break;
                }
                
                if (confirmAction("Are you sure you want to delete this user?")) {
                    User* user = findUserByUsername(username);
                    if (user == NULL) {
                        centerText("User not found!");
                        pauseScreen();
                        break;
                    }
                    
                    user->isActive = 0;
                    if (updateUserInFile(user)) {
                        centerText("User deleted successfully!");
                    } else {
                        centerText("Failed to delete user!");
                    }
                    free(user);
                }
                pauseScreen();
                break;
            }
            case 4: {
                char username[MAX_USERNAME];
                getStringInput("Enter username to search: ", username, sizeof(username));
                
                User* user = findUserByUsername(username);
                if (user == NULL) {
                    centerText("User not found!");
                } else {
                    clearScreen();
                    centerTitle("USER DETAILS");
                    printf("\n");
                    char info[200];
                    sprintf(info, "ID: %s", user->id);
                    centerText(info);
                    sprintf(info, "Username: %s", user->username);
                    centerText(info);
                    sprintf(info, "Full Name: %s", user->fullName);
                    centerText(info);
                    sprintf(info, "Email: %s", user->email);
                    centerText(info);
                    sprintf(info, "Phone: %s", user->phone);
                    centerText(info);
                    sprintf(info, "Address: %s", user->address);
                    centerText(info);
                    sprintf(info, "Role: %s", user->role == ROLE_ADMIN ? "Admin" : 
                           (user->role == ROLE_SELLER ? "Seller" : "Buyer"));
                    centerText(info);
                    sprintf(info, "Status: %s", user->isActive ? "Active" : "Inactive");
                    centerText(info);
                    free(user);
                }
                pauseScreen();
                break;
            }
            case 5:
                return;
            default:
                centerText("Invalid choice!");
                pauseScreen();
        }
    }
}

void adminProductManagement() {
    int choice;
    
    while (1) {
        clearScreen();
        centerTitle("ADMIN PRODUCT MANAGEMENT");
        
        const char* options[] = {
            "1. View All Products",
            "2. Delete Product",
            "3. View Products by Category",
            "4. Search Product",
            "5. Back to Dashboard"
        };
        
        for (int i = 0; i < 5; i++) {
            centerText(options[i]);
        }
        
        choice = getIntegerInput("Enter your choice: ");
        
        switch (choice) {
            case 1: {
                Product* products = NULL;
                int count = 0;
                if (loadAllProducts(&products, &count)) {
                    clearScreen();
                    centerTitle("ALL PRODUCTS");
                    printf("\n");
                    centerText("ID        Name                      Category      Price      Stock");
                    centerText("------------------------------------------------------------");
                    
                    for (int i = 0; i < count; i++) {
                        if (products[i].isActive) {
                            char line[200];
                            sprintf(line, "%-10s %-25s %-15s BDT %-9.2f %-10d", 
                                   products[i].id, products[i].name, 
                                   products[i].category, products[i].price, products[i].stock);
                            centerText(line);
                        }
                    }
                    free(products);
                } else {
                    centerText("No products found!");
                }
                pauseScreen();
                break;
            }
            case 2: {
                char productId[MAX_ID];
                getStringInput("Enter product ID to delete: ", productId, sizeof(productId));
                
                if (confirmAction("Are you sure you want to delete this product?")) {
                    if (deleteProductFromFile(productId)) {
                        centerText("Product deleted successfully!");
                    } else {
                        centerText("Product not found or deletion failed!");
                    }
                }
                pauseScreen();
                break;
            }
            case 3: {
                char category[MAX_CATEGORY];
                getStringInput("Enter category: ", category, sizeof(category));
                
                Product* results = NULL;
                int count = 0;
                if (searchProductsByCategory(category, &results, &count)) {
                    clearScreen();
                    char title[100];
                    sprintf(title, "PRODUCTS IN CATEGORY: %s", category);
                    centerTitle(title);
                    printf("\n");
                    centerText("ID        Name                      Price      Stock");
                    centerText("------------------------------------------------------------");
                    
                    for (int i = 0; i < count; i++) {
                        char line[200];
                        sprintf(line, "%-10s %-25s BDT %-9.2f %-10d", 
                               results[i].id, results[i].name, 
                               results[i].price, results[i].stock);
                        centerText(line);
                    }
                    free(results);
                } else {
                    centerText("No products found in this category!");
                }
                pauseScreen();
                break;
            }
            case 4: {
                char searchTerm[MAX_PRODUCT_NAME];
                getStringInput("Enter product name to search: ", searchTerm, sizeof(searchTerm));
                
                Product* product = findProductByName(searchTerm);
                if (product != NULL) {
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
                    sprintf(info, "Stock: %d", product->stock);
                    centerText(info);
                    sprintf(info, "Sold: %d", product->soldCount);
                    centerText(info);
                    free(product);
                } else {
                    centerText("Product not found!");
                }
                pauseScreen();
                break;
            }
            case 5:
                return;
            default:
                centerText("Invalid choice!");
                pauseScreen();
        }
    }
}

void viewSalesReport() {
    clearScreen();
    centerTitle("SALES REPORT");
    
    SalesReport* reports = NULL;
    int count = 0;
    getBestSellingProducts(&reports, &count);
    
    if (count > 0) {
        printf("\n");
        centerText("BEST SELLING PRODUCTS:");
        centerText("Product Name               Units Sold    Revenue");
        centerText("------------------------------------------------------------");
        
        for (int i = 0; i < count && i < 10; i++) {
            char line[200];
            sprintf(line, "%-25s %-15d BDT %-14.2f", 
                   reports[i].productName, 
                   reports[i].totalSold,
                   reports[i].totalRevenue);
            centerText(line);
        }
        free(reports);
    } else {
        centerText("No sales data available.");
    }
    
    double totalRevenue = calculateTotalRevenue();
    char msg[100];
    sprintf(msg, "Total Revenue: BDT %.2f", totalRevenue);
    centerText("");
    centerText(msg);
    
    pauseScreen();
}

void viewAllOrders() {
    clearScreen();
    centerTitle("ALL ORDERS");
    
    Order* orders = NULL;
    int count = 0;
    
    if (loadAllOrders(&orders, &count)) {
        printf("\n");
        centerText("Order ID   Buyer         Seller        Product               Qty    Total");
        centerText("------------------------------------------------------------");
        
        for (int i = 0; i < count; i++) {
            char line[200];
            sprintf(line, "%-10s %-12s %-12s %-20s %-8d BDT %-9.2f", 
                   orders[i].id, orders[i].buyerId, orders[i].sellerId,
                   orders[i].productName, orders[i].quantity, orders[i].total);
            centerText(line);
        }
        free(orders);
    } else {
        centerText("No orders found!");
    }
    
    pauseScreen();
}

void manageShippingCharge() {
    clearScreen();
    centerTitle("MANAGE SHIPPING CHARGE");
    
    char info[200];
    sprintf(info, "Current Global Shipping Charge: BDT %.2f", globalShippingCharge);
    centerText(info);
    centerText("");
    centerText("Note: This shipping charge applies to ALL orders.");
    centerText("It is a flat charge per order, regardless of items or sellers.");
    printf("\n");
    
    double newCharge = getDoubleInput("Enter new shipping charge (BDT): ");
    
    if (newCharge < 0) {
        centerText("Shipping charge cannot be negative!");
        pauseScreen();
        return;
    }
    
    globalShippingCharge = newCharge;
    sprintf(info, "Shipping charge updated successfully to BDT %.2f", globalShippingCharge);
    centerText(info);
    centerText("This will apply to all new orders.");
    
    pauseScreen();
}