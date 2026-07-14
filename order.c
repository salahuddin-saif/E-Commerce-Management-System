#include "functions.h"

int saveOrder(const Order* order) {
    if (order == NULL) return 0;
    
    FILE* fp = fopen(ORDERS_FILE, "a");
    if (fp == NULL) return 0;
    
    fprintf(fp, "%s,%s,%s,%s,%s,%d,%.2f,%.2f,%s,%ld,%s,%.2f\n",
            order->id, order->buyerId, order->sellerId,
            order->productId, order->productName,
            order->quantity, order->price, order->total,
            order->status, order->orderDate, order->shippingAddress,
            order->shippingCharge);
    
    fclose(fp);
    return 1;
}

int loadAllOrders(Order** orders, int* count) {
    FILE* fp = fopen(ORDERS_FILE, "r");
    if (fp == NULL) {
        *orders = NULL;
        *count = 0;
        return 0;
    }
    
    Order temp;
    int total = 0;
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%lf,%lf,%[^,],%ld,%[^,],%lf\n",
                  temp.id, temp.buyerId, temp.sellerId,
                  temp.productId, temp.productName,
                  &temp.quantity, &temp.price, &temp.total,
                  temp.status, &temp.orderDate, temp.shippingAddress,
                  &temp.shippingCharge) == 12) {
        total++;
    }
    
    if (total == 0) {
        fclose(fp);
        *orders = NULL;
        *count = 0;
        return 0;
    }
    
    *orders = (Order*)malloc(total * sizeof(Order));
    if (*orders == NULL) {
        fclose(fp);
        return 0;
    }
    
    rewind(fp);
    int i = 0;
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%lf,%lf,%[^,],%ld,%[^,],%lf\n",
                  (*orders)[i].id, (*orders)[i].buyerId, (*orders)[i].sellerId,
                  (*orders)[i].productId, (*orders)[i].productName,
                  &(*orders)[i].quantity, &(*orders)[i].price, &(*orders)[i].total,
                  (*orders)[i].status, &(*orders)[i].orderDate, (*orders)[i].shippingAddress,
                  &(*orders)[i].shippingCharge) == 12) {
        i++;
    }
    
    fclose(fp);
    *count = total;
    return 1;
}

int loadOrdersByBuyer(const char* buyerId, Order** orders, int* count) {
    if (buyerId == NULL) {
        *orders = NULL;
        *count = 0;
        return 0;
    }
    
    FILE* fp = fopen(ORDERS_FILE, "r");
    if (fp == NULL) {
        *orders = NULL;
        *count = 0;
        return 0;
    }
    
    Order* tempOrders = NULL;
    int tempCount = 0;
    Order current;
    
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%lf,%lf,%[^,],%ld,%[^,],%lf\n",
                  current.id, current.buyerId, current.sellerId,
                  current.productId, current.productName,
                  &current.quantity, &current.price, &current.total,
                  current.status, &current.orderDate, current.shippingAddress,
                  &current.shippingCharge) == 12) {
        
        if (strcmp(current.buyerId, buyerId) == 0) {
            tempOrders = (Order*)realloc(tempOrders, (tempCount + 1) * sizeof(Order));
            tempOrders[tempCount] = current;
            tempCount++;
        }
    }
    
    fclose(fp);
    
    if (tempCount > 0) {
        *orders = tempOrders;
        *count = tempCount;
        return 1;
    } else {
        *orders = NULL;
        *count = 0;
        return 0;
    }
}

int loadOrdersBySeller(const char* sellerId, Order** orders, int* count) {
    if (sellerId == NULL) {
        *orders = NULL;
        *count = 0;
        return 0;
    }
    
    FILE* fp = fopen(ORDERS_FILE, "r");
    if (fp == NULL) {
        *orders = NULL;
        *count = 0;
        return 0;
    }
    
    Order* tempOrders = NULL;
    int tempCount = 0;
    Order current;
    
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%lf,%lf,%[^,],%ld,%[^,],%lf\n",
                  current.id, current.buyerId, current.sellerId,
                  current.productId, current.productName,
                  &current.quantity, &current.price, &current.total,
                  current.status, &current.orderDate, current.shippingAddress,
                  &current.shippingCharge) == 12) {
        
        if (strcmp(current.sellerId, sellerId) == 0) {
            tempOrders = (Order*)realloc(tempOrders, (tempCount + 1) * sizeof(Order));
            tempOrders[tempCount] = current;
            tempCount++;
        }
    }
    
    fclose(fp);
    
    if (tempCount > 0) {
        *orders = tempOrders;
        *count = tempCount;
        return 1;
    } else {
        *orders = NULL;
        *count = 0;
        return 0;
    }
}

int updateOrderStatus(const char* orderId, const char* status) {
    if (orderId == NULL || status == NULL) return 0;
    
    FILE* fp = fopen(ORDERS_FILE, "r");
    if (fp == NULL) return 0;
    
    FILE* temp = fopen("temp_orders.txt", "w");
    if (temp == NULL) {
        fclose(fp);
        return 0;
    }
    
    Order current;
    int found = 0;
    
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%lf,%lf,%[^,],%ld,%[^,],%lf\n",
                  current.id, current.buyerId, current.sellerId,
                  current.productId, current.productName,
                  &current.quantity, &current.price, &current.total,
                  current.status, &current.orderDate, current.shippingAddress,
                  &current.shippingCharge) == 12) {
        
        if (strcmp(current.id, orderId) == 0) {
            strcpy(current.status, status);
            found = 1;
        }
        
        fprintf(temp, "%s,%s,%s,%s,%s,%d,%.2f,%.2f,%s,%ld,%s,%.2f\n",
                current.id, current.buyerId, current.sellerId,
                current.productId, current.productName,
                current.quantity, current.price, current.total,
                current.status, current.orderDate, current.shippingAddress,
                current.shippingCharge);
    }
    
    fclose(fp);
    fclose(temp);
    
    if (found) {
        remove(ORDERS_FILE);
        rename("temp_orders.txt", ORDERS_FILE);
        return 1;
    } else {
        remove("temp_orders.txt");
        return 0;
    }
}

Order* findOrderById(const char* orderId) {
    if (orderId == NULL) return NULL;
    
    FILE* fp = fopen(ORDERS_FILE, "r");
    if (fp == NULL) return NULL;
    
    Order order;
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%lf,%lf,%[^,],%ld,%[^,],%lf\n",
                  order.id, order.buyerId, order.sellerId,
                  order.productId, order.productName,
                  &order.quantity, &order.price, &order.total,
                  order.status, &order.orderDate, order.shippingAddress,
                  &order.shippingCharge) == 12) {
        if (strcmp(order.id, orderId) == 0) {
            fclose(fp);
            Order* foundOrder = (Order*)malloc(sizeof(Order));
            *foundOrder = order;
            return foundOrder;
        }
    }
    
    fclose(fp);
    return NULL;
}

void generateInvoice(const Order* order) {
    if (order == NULL) return;
    
    clearScreen();
    centerTitle("INVOICE");
    
    printf("\n");
    char info[200];
    sprintf(info, "Order ID: %s", order->id);
    centerText(info);
    sprintf(info, "Date: %s", ctime(&order->orderDate));
    centerText(info);
    centerText("------------------------------------------------");
    sprintf(info, "Product: %s", order->productName);
    centerText(info);
    sprintf(info, "Product ID: %s", order->productId);
    centerText(info);
    sprintf(info, "Quantity: %d", order->quantity);
    centerText(info);
    sprintf(info, "Price per unit: BDT %.2f", order->price);
    centerText(info);
    centerText("------------------------------------------------");
    sprintf(info, "Subtotal: BDT %.2f", order->price * order->quantity);
    centerText(info);
    sprintf(info, "Shipping Charge: BDT %.2f", order->shippingCharge);
    centerText(info);
    centerText("------------------------------------------------");
    sprintf(info, "TOTAL: BDT %.2f", order->total);
    centerText(info);
    centerText("------------------------------------------------");
    sprintf(info, "Shipping Address: %s", order->shippingAddress);
    centerText(info);
    centerText("------------------------------------------------");
    sprintf(info, "Status: %s", order->status);
    centerText(info);
    centerText("================================================");
    centerText("Thank you for your order!");
}

void viewOrderHistory(const char* userId, int role) {
    if (userId == NULL) return;
    
    clearScreen();
    centerTitle("ORDER HISTORY");
    
    Order* orders = NULL;
    int count = 0;
    int success = 0;
    
    if (role == ROLE_BUYER) {
        success = loadOrdersByBuyer(userId, &orders, &count);
    } else if (role == ROLE_SELLER) {
        success = loadOrdersBySeller(userId, &orders, &count);
    }
    
    if (!success || count == 0) {
        centerText("No orders found.");
        pauseScreen();
        return;
    }
    
    printf("\n");
    centerText("Order ID     Product               Qty    Total      Status");
    centerText("------------------------------------------------------------");
    
    for (int i = 0; i < count; i++) {
        char line[200];
        sprintf(line, "%-12s %-20s %-8d BDT %-8.2f %-12s", 
               orders[i].id, orders[i].productName,
               orders[i].quantity, orders[i].total, orders[i].status);
        centerText(line);
    }
    
    printf("\n");
    centerText("1. View Invoice");
    if (role == ROLE_SELLER) {
        centerText("2. Update Order Status");
        centerText("3. Back");
    } else {
        centerText("2. Back");
    }
    int choice = getIntegerInput("Enter choice: ");
    
    if (choice == 1) {
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
    } else if (choice == 2 && role == ROLE_SELLER) {
        char orderId[MAX_ID];
        getStringInput("Enter Order ID to update: ", orderId, sizeof(orderId));
        
        Order* order = findOrderById(orderId);
        if (order == NULL) {
            centerText("Order not found!");
            free(orders);
            pauseScreen();
            return;
        }
        
        if (strcmp(order->sellerId, userId) != 0) {
            centerText("You can only update status for orders of your products!");
            free(order);
            free(orders);
            pauseScreen();
            return;
        }
        free(order);
        
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
        pauseScreen();
    }
    
    free(orders);
}

int getNextOrderNumber() {
    Order* orders = NULL;
    int count = 0;
    int maxNum = 0;
    
    if (loadAllOrders(&orders, &count)) {
        for (int i = 0; i < count; i++) {
            int num;
            if (sscanf(orders[i].id, "ORD%d", &num) == 1) {
                if (num > maxNum) maxNum = num;
            }
        }
        free(orders);
    }
    
    return maxNum + 1;
}