#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#endif

// Maximum lengths
#define MAX_NAME 50
#define MAX_USERNAME 30
#define MAX_PASSWORD 30
#define MAX_EMAIL 50
#define MAX_PHONE 15
#define MAX_ADDRESS 100
#define MAX_PRODUCT_NAME 100
#define MAX_DESCRIPTION 500
#define MAX_CATEGORY 30
#define MAX_ID 20

// Role definitions
#define ROLE_ADMIN 1
#define ROLE_SELLER 2
#define ROLE_BUYER 3

// Status definitions
#define STATUS_PENDING 0
#define STATUS_APPROVED 1
#define STATUS_REJECTED 2

// File paths
#define ACCOUNTS_FILE "accounts.txt"
#define PRODUCTS_FILE "products.txt"
#define ORDERS_FILE "orders.txt"
#define CART_FILE "cart.txt"
#define SALES_FILE "sales.txt"

// User struct
typedef struct {
    char id[MAX_ID];
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char email[MAX_EMAIL];
    char fullName[MAX_NAME];
    char phone[MAX_PHONE];
    char address[MAX_ADDRESS];
    int role; // 1=Admin, 2=Seller, 3=Buyer
    int status; // 0=Pending, 1=Approved, 2=Rejected
    int isActive;
    time_t createdAt;
} User;

// Product struct
typedef struct {
    char id[MAX_ID];
    char name[MAX_PRODUCT_NAME];
    char description[MAX_DESCRIPTION];
    char category[MAX_CATEGORY];
    char sellerId[MAX_ID];
    double price;
    int stock;
    int soldCount;
    int isActive;
    time_t createdAt;
    time_t updatedAt;
} Product;

// Cart Item struct
typedef struct {
    char productId[MAX_ID];
    char buyerId[MAX_ID];
    int quantity;
    time_t addedAt;
} CartItem;

// Order struct
typedef struct {
    char id[MAX_ID];
    char buyerId[MAX_ID];
    char sellerId[MAX_ID];
    char productId[MAX_ID];
    char productName[MAX_PRODUCT_NAME];
    int quantity;
    double price;
    double total;
    char status[20]; // "Pending", "Shipped", "Delivered", "Cancelled"
    time_t orderDate;
    char shippingAddress[MAX_ADDRESS];
    double shippingCharge;
} Order;

// Sales Report struct
typedef struct {
    char productId[MAX_ID];
    char productName[MAX_PRODUCT_NAME];
    int totalSold;
    double totalRevenue;
    double averagePrice;
} SalesReport;

// Global variables
extern User currentUser;
extern int currentUserRole;
extern double globalShippingCharge;

// ============= Utility Functions =============
char* generateId(const char* prefix);
char* getCurrentDateTime();
int validateEmail(const char* email);
int validatePhone(const char* phone);
int validatePassword(const char* password);
void clearScreen();
void pauseScreen();
int confirmAction(const char* message);
int fileExists(const char* filename);
void trimString(char* str);
void centerText(const char* text);
void centerTitle(const char* title);
void centerMenu(const char* options[], int count);
int getIntegerInput(const char* prompt);
double getDoubleInput(const char* prompt);
void getStringInput(const char* prompt, char* buffer, int size);

// ============= Account Functions =============
int login();
void logout();
int registerUser();
int changePassword();
int validateUserCredentials(const char* username, const char* password);
User* findUserByUsername(const char* username);
int saveUser(const User* user);
int loadAllUsers(User** users, int* count);
int updateUserInFile(const User* user);
int isUsernameTaken(const char* username);

// ============= Admin Functions =============
void adminDashboard();
void approveSellers();
void manageUsers();
void adminProductManagement();
void viewSalesReport();
void viewAllOrders();
void manageShippingCharge();

// ============= Seller Functions =============
void sellerDashboard();
void sellerProductManagement();
void addProduct();
void editProduct();
void deleteProduct();
void viewSellerProducts();
void updateStock();
void viewSellerOrders();
void viewSellerSales();

// ============= Buyer Functions =============
void buyerDashboard();
void browseProducts();
void searchProducts();
void sortProducts();
void viewProductDetails(const char* productId);

// ============= Product Functions =============
int saveProduct(const Product* product);
int loadAllProducts(Product** products, int* count);
int updateProductInFile(const Product* product);
int deleteProductFromFile(const char* productId);
Product* findProductById(const char* productId);
Product* findProductByName(const char* name);
int searchProductsByCategory(const char* category, Product** results, int* count);
int searchProductsBySeller(const char* sellerId, Product** results, int* count);
int validateProduct(const Product* product);
int updateProductStock(const char* productId, int newStock);

// ============= Cart Functions =============
int addToCart(const char* productId, const char* buyerId, int quantity);
int removeFromCart(const char* productId, const char* buyerId);
int updateCartQuantity(const char* productId, const char* buyerId, int newQuantity);
int viewCart(const char* buyerId);
int clearCart(const char* buyerId);
int checkout(const char* buyerId);
int loadCartItems(const char* buyerId, CartItem** items, int* count);
int saveCartItem(const CartItem* item);
int removeCartItemFromFile(const char* productId, const char* buyerId);

// ============= Order Functions =============
int saveOrder(const Order* order);
int loadAllOrders(Order** orders, int* count);
int loadOrdersByBuyer(const char* buyerId, Order** orders, int* count);
int loadOrdersBySeller(const char* sellerId, Order** orders, int* count);
int updateOrderStatus(const char* orderId, const char* status);
Order* findOrderById(const char* orderId);
void generateInvoice(const Order* order);
void viewOrderHistory(const char* userId, int role);
int getNextOrderNumber();

// ============= Report Functions =============
void generateSalesReport();
void generateRevenueReport();
void getBestSellingProducts(SalesReport** reports, int* count);
void getMostSoldProducts(SalesReport** reports, int* count);
double calculateTotalRevenue();
void generateProductSalesReport(const char* productId);
void exportReportToFile();

#endif