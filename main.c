#include "functions.h"

// Global variable definitions
User currentUser;
int currentUserRole = 0;
double globalShippingCharge = 50.00; // Default shipping charge managed by admin

void initializeSystem() {
    if (!fileExists(ACCOUNTS_FILE)) {
        FILE* fp = fopen(ACCOUNTS_FILE, "w");
        if (fp) fclose(fp);
    }
    if (!fileExists(PRODUCTS_FILE)) {
        FILE* fp = fopen(PRODUCTS_FILE, "w");
        if (fp) fclose(fp);
    }
    if (!fileExists(ORDERS_FILE)) {
        FILE* fp = fopen(ORDERS_FILE, "w");
        if (fp) fclose(fp);
    }
    if (!fileExists(CART_FILE)) {
        FILE* fp = fopen(CART_FILE, "w");
        if (fp) fclose(fp);
    }
    if (!fileExists(SALES_FILE)) {
        FILE* fp = fopen(SALES_FILE, "w");
        if (fp) fclose(fp);
    }
    
    User* users = NULL;
    int count = 0;
    if (loadAllUsers(&users, &count) == 0 || count == 0) {
        User admin;
        strcpy(admin.id, generateId("ADM"));
        strcpy(admin.username, "admin");
        strcpy(admin.password, "admin123");
        strcpy(admin.email, "admin@system.com");
        strcpy(admin.fullName, "System Administrator");
        strcpy(admin.phone, "0000000000");
        strcpy(admin.address, "System");
        admin.role = ROLE_ADMIN;
        admin.status = STATUS_APPROVED;
        admin.isActive = 1;
        admin.createdAt = time(NULL);
        saveUser(&admin);
        centerText("Default admin account created!");
        centerText("Username: admin");
        centerText("Password: admin123");
        printf("\n");
        pauseScreen();
    }
    
    if (users) free(users);
}

int main() {
    initializeSystem();
    
    int choice;
    int loggedIn = 0;
    
    while (1) {
        if (!loggedIn) {
            clearScreen();
            printf("\n");
            centerTitle("E-COMMERCE MANAGEMENT SYSTEM");
            printf("\n");
            
            const char* options[] = {
                "1. Login",
                "2. Register",
                "3. Exit"
            };
            
            for (int i = 0; i < 3; i++) {
                centerText(options[i]);
            }
            
            centerText("Enter your choice: ");
            printf("\n\t\t\t\t\t");
            scanf("%d", &choice);
            getchar();
            
            switch (choice) {
                case 1:
                    if (login()) {
                        loggedIn = 1;
                        if (currentUser.role == ROLE_ADMIN) {
                            adminDashboard();
                        } else if (currentUser.role == ROLE_SELLER) {
                            sellerDashboard();
                        } else if (currentUser.role == ROLE_BUYER) {
                            buyerDashboard();
                        }
                        loggedIn = 0;
                    }
                    break;
                case 2:
                    if (registerUser()) {
                        centerText("Registration successful! Please login.");
                        pauseScreen();
                    }
                    break;
                case 3:
                    centerText("Thank you for using the system. Goodbye!");
                    exit(0);
                default:
                    centerText("Invalid choice! Please try again.");
                    pauseScreen();
            }
        }
    }
    
    return 0;
}