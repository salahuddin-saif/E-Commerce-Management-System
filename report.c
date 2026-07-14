#include "functions.h"

void generateSalesReport() {
    clearScreen();
    centerTitle("SALES REPORT");
    
    Order* orders = NULL;
    int count = 0;
    
    if (!loadAllOrders(&orders, &count) || count == 0) {
        centerText("No sales data available.");
        pauseScreen();
        return;
    }
    
    int totalOrders = count;
    double totalRevenue = 0;
    int totalItems = 0;
    double totalShipping = 0;
    
    for (int i = 0; i < count; i++) {
        totalRevenue += orders[i].total;
        totalItems += orders[i].quantity;
        totalShipping += orders[i].shippingCharge;
    }
    
    printf("\n");
    centerText("SALES SUMMARY");
    centerText("------------------------------------------------");
    char info[200];
    sprintf(info, "Total Orders: %d", totalOrders);
    centerText(info);
    sprintf(info, "Total Items Sold: %d", totalItems);
    centerText(info);
    sprintf(info, "Total Revenue: BDT %.2f", totalRevenue);
    centerText(info);
    sprintf(info, "Total Shipping Collected: BDT %.2f", totalShipping);
    centerText(info);
    sprintf(info, "Average Order Value: BDT %.2f", totalRevenue / totalOrders);
    centerText(info);
    centerText("------------------------------------------------");
    
    SalesReport* reports = NULL;
    int reportCount = 0;
    getBestSellingProducts(&reports, &reportCount);
    
    if (reportCount > 0) {
        printf("\n");
        centerText("TOP 5 BEST SELLING PRODUCTS");
        centerText("Product Name               Units Sold    Revenue");
        centerText("------------------------------------------------");
        
        int limit = reportCount < 5 ? reportCount : 5;
        for (int i = 0; i < limit; i++) {
            char line[200];
            sprintf(line, "%-25s %-15d BDT %-14.2f", 
                   reports[i].productName,
                   reports[i].totalSold,
                   reports[i].totalRevenue);
            centerText(line);
        }
        free(reports);
    }
    
    free(orders);
    pauseScreen();
}

void generateRevenueReport() {
    clearScreen();
    centerTitle("REVENUE REPORT");
    
    Order* orders = NULL;
    int count = 0;
    
    if (!loadAllOrders(&orders, &count) || count == 0) {
        centerText("No revenue data available.");
        pauseScreen();
        return;
    }
    
    double totalRevenue = calculateTotalRevenue();
    char info[200];
    sprintf(info, "Total Revenue: BDT %.2f", totalRevenue);
    centerText(info);
    
    // Calculate revenue by month
    printf("\n");
    centerText("REVENUE BY MONTH");
    centerText("Month                Orders    Revenue");
    centerText("------------------------------------------------");
    
    int months[12] = {0};
    double monthlyRevenue[12] = {0};
    
    for (int i = 0; i < count; i++) {
        struct tm* tm_info = localtime(&orders[i].orderDate);
        int month = tm_info->tm_mon;
        months[month]++;
        monthlyRevenue[month] += orders[i].total;
    }
    
    const char* monthNames[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    
    for (int i = 0; i < 12; i++) {
        if (months[i] > 0) {
            char line[200];
            sprintf(line, "%-20s %-8d BDT %-12.2f", 
                   monthNames[i], months[i], monthlyRevenue[i]);
            centerText(line);
        }
    }
    
    printf("\n");
    centerText("REVENUE BY PRODUCT");
    centerText("Product Name               Revenue");
    centerText("------------------------------------------------");
    
    SalesReport* reports = NULL;
    int reportCount = 0;
    getBestSellingProducts(&reports, &reportCount);
    
    if (reportCount > 0) {
        for (int i = 0; i < reportCount && i < 10; i++) {
            char line[200];
            sprintf(line, "%-25s BDT %-14.2f", 
                   reports[i].productName,
                   reports[i].totalRevenue);
            centerText(line);
        }
        free(reports);
    }
    
    free(orders);
    pauseScreen();
}

void getBestSellingProducts(SalesReport** reports, int* count) {
    Product* products = NULL;
    int productCount = 0;
    
    if (!loadAllProducts(&products, &productCount) || productCount == 0) {
        *reports = NULL;
        *count = 0;
        return;
    }
    
    SalesReport* tempReports = NULL;
    int tempCount = 0;
    
    for (int i = 0; i < productCount; i++) {
        if (products[i].soldCount > 0 && products[i].isActive) {
            tempReports = (SalesReport*)realloc(tempReports, (tempCount + 1) * sizeof(SalesReport));
            strcpy(tempReports[tempCount].productId, products[i].id);
            strcpy(tempReports[tempCount].productName, products[i].name);
            tempReports[tempCount].totalSold = products[i].soldCount;
            tempReports[tempCount].totalRevenue = products[i].soldCount * products[i].price;
            tempReports[tempCount].averagePrice = products[i].price;
            tempCount++;
        }
    }
    
    free(products);
    
    if (tempCount == 0) {
        *reports = NULL;
        *count = 0;
        return;
    }
    
    for (int i = 0; i < tempCount - 1; i++) {
        for (int j = 0; j < tempCount - i - 1; j++) {
            if (tempReports[j].totalSold < tempReports[j + 1].totalSold) {
                SalesReport temp = tempReports[j];
                tempReports[j] = tempReports[j + 1];
                tempReports[j + 1] = temp;
            }
        }
    }
    
    *reports = tempReports;
    *count = tempCount;
}

void getMostSoldProducts(SalesReport** reports, int* count) {
    getBestSellingProducts(reports, count);
}

double calculateTotalRevenue() {
    Order* orders = NULL;
    int count = 0;
    double total = 0;
    
    if (loadAllOrders(&orders, &count)) {
        for (int i = 0; i < count; i++) {
            total += orders[i].total;
        }
        free(orders);
    }
    
    return total;
}

void generateProductSalesReport(const char* productId) {
    if (productId == NULL) {
        centerText("Invalid product ID.");
        return;
    }
    
    clearScreen();
    centerTitle("PRODUCT SALES REPORT");
    
    Product* product = findProductById(productId);
    if (product == NULL) {
        centerText("Product not found!");
        pauseScreen();
        return;
    }
    
    printf("\n");
    char info[200];
    sprintf(info, "Product: %s", product->name);
    centerText(info);
    sprintf(info, "Category: %s", product->category);
    centerText(info);
    sprintf(info, "Price: BDT %.2f", product->price);
    centerText(info);
    sprintf(info, "Total Sold: %d", product->soldCount);
    centerText(info);
    sprintf(info, "Revenue Generated: BDT %.2f", product->soldCount * product->price);
    centerText(info);
    sprintf(info, "Current Stock: %d", product->stock);
    centerText(info);
    sprintf(info, "Status: %s", product->isActive ? "Active" : "Inactive");
    centerText(info);
    
    Order* orders = NULL;
    int count = 0;
    
    if (loadAllOrders(&orders, &count)) {
        int orderCount = 0;
        double totalRevenue = 0;
        double totalShipping = 0;
        
        printf("\n");
        centerText("ORDER HISTORY FOR THIS PRODUCT");
        centerText("Order ID     Buyer         Qty    Total      Shipping");
        centerText("------------------------------------------------------------");
        
        for (int i = 0; i < count; i++) {
            if (strcmp(orders[i].productId, productId) == 0) {
                char line[200];
                sprintf(line, "%-12s %-12s %-8d BDT %-8.2f BDT %-8.2f",
                       orders[i].id, orders[i].buyerId,
                       orders[i].quantity, orders[i].total,
                       orders[i].shippingCharge);
                centerText(line);
                orderCount++;
                totalRevenue += orders[i].total;
                totalShipping += orders[i].shippingCharge;
            }
        }
        
        centerText("------------------------------------------------------------");
        sprintf(info, "Total Orders: %d", orderCount);
        centerText(info);
        sprintf(info, "Total Revenue: BDT %.2f", totalRevenue);
        centerText(info);
        sprintf(info, "Total Shipping: BDT %.2f", totalShipping);
        centerText(info);
        free(orders);
    }
    
    free(product);
    pauseScreen();
}

void exportReportToFile() {
    clearScreen();
    centerTitle("EXPORT REPORT");
    
    FILE* fp = fopen(SALES_FILE, "w");
    if (fp == NULL) {
        centerText("Failed to create sales report file!");
        pauseScreen();
        return;
    }
    
    fprintf(fp, "========================================\n");
    fprintf(fp, "        SALES REPORT\n");
    fprintf(fp, "========================================\n");
    fprintf(fp, "Generated: %s\n", getCurrentDateTime());
    fprintf(fp, "========================================\n\n");
    
    Order* orders = NULL;
    int count = 0;
    
    if (loadAllOrders(&orders, &count) && count > 0) {
        double totalRevenue = 0;
        int totalItems = 0;
        double totalShipping = 0;
        
        for (int i = 0; i < count; i++) {
            totalRevenue += orders[i].total;
            totalItems += orders[i].quantity;
            totalShipping += orders[i].shippingCharge;
        }
        
        fprintf(fp, "SUMMARY:\n");
        fprintf(fp, "Total Orders: %d\n", count);
        fprintf(fp, "Total Items Sold: %d\n", totalItems);
        fprintf(fp, "Total Revenue: BDT %.2f\n", totalRevenue);
        fprintf(fp, "Total Shipping Collected: BDT %.2f\n", totalShipping);
        fprintf(fp, "Average Order Value: BDT %.2f\n\n", totalRevenue / count);
        
        SalesReport* reports = NULL;
        int reportCount = 0;
        getBestSellingProducts(&reports, &reportCount);
        
        if (reportCount > 0) {
            fprintf(fp, "TOP 10 BEST SELLING PRODUCTS:\n");
            fprintf(fp, "%-30s %-15s %-15s\n", "Product", "Units Sold", "Revenue");
            fprintf(fp, "----------------------------------------\n");
            
            int limit = reportCount < 10 ? reportCount : 10;
            for (int i = 0; i < limit; i++) {
                fprintf(fp, "%-30s %-15d BDT %-14.2f\n", 
                        reports[i].productName,
                        reports[i].totalSold,
                        reports[i].totalRevenue);
            }
            free(reports);
        }
        
        fprintf(fp, "\n========================================\n");
        fprintf(fp, "DETAILED ORDERS:\n");
        fprintf(fp, "========================================\n");
        fprintf(fp, "%-12s %-12s %-12s %-20s %-8s %-10s %-10s %-10s\n",
                "Order ID", "Buyer", "Seller", "Product", "Qty", "Total", "Shipping", "Status");
        fprintf(fp, "----------------------------------------\n");
        
        for (int i = 0; i < count; i++) {
            fprintf(fp, "%-12s %-12s %-12s %-20s %-8d BDT %-8.2f BDT %-8.2f %-10s\n",
                    orders[i].id, orders[i].buyerId, orders[i].sellerId,
                    orders[i].productName, orders[i].quantity,
                    orders[i].total, orders[i].shippingCharge,
                    orders[i].status);
        }
        
        free(orders);
    } else {
        fprintf(fp, "No sales data available.\n");
    }
    
    fclose(fp);
    char msg[100];
    sprintf(msg, "Report exported successfully to %s", SALES_FILE);
    centerText(msg);
    pauseScreen();
}