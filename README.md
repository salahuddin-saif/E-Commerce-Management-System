# 🛒 E-Commerce Management System

A complete console-based E-Commerce Management System developed in C with role-based access control for Administrators, Sellers, and Buyers.

## 📋 Table of Contents
- [Features](#-features)
- [Tech Stack](#-tech-stack)
- [Installation](#-installation)
- [Default Admin Account](#-default-admin-account)
- [How to Use](#-how-to-use)
- [Project Structure](#-project-structure)
- [Data Files](#-data-files)
- [Screenshots](#-screenshots)
- [License](#-license)
- [Contributing](#-contributing)

---

## ✨ Features

### 👑 Admin Dashboard
- Approve/Reject Seller Registrations
- Manage Users (Activate/Deactivate/Delete)
- View All Products
- View All Orders
- Manage Global Shipping Charge
- View Sales Reports & Revenue Analytics
- Change Password

### 🏪 Seller Dashboard
- Add/Edit/Delete Products
- Update Stock Inventory
- View My Products
- View Order History
- Update Order Status (Pending → Shipped → Delivered → Cancelled)
- View Personal Sales Reports
- Change Password

### 🛒 Buyer Dashboard
- Browse All Available Products
- Search Products by Name, Category, or Price Range
- Sort Products by Price, Name, or Stock
- View Product Details
- Add to Cart
- Update Cart Quantities
- Remove Items from Cart
- Checkout with Flat Shipping Charge
- View Order History
- Generate and View Invoices
- Change Password

---

## 🛠️ Tech Stack

- **Language**: C (ISO C99)
- **Storage**: File-based (No database required)
- **Platform**: Cross-platform (Windows, Linux, macOS)
- **Compiler**: GCC / Clang

---

## 📦 Installation

### Prerequisites
- GCC or Clang compiler installed on your system

### Compilation

#### Option 1: Using GCC
```bash
gcc -o ecom main.c account.c admin.c seller.c buyer.c product.c cart.c order.c report.c utility.c

```

Option 2: Using Code::Blocks IDE
Create a new Console Application project

Add all .c files to the project

Keep functions.h in the same folder

Build and Run (F9)

Running the Application
Linux/macOS
bash
./ecom
Windows
bash
ecom.exe
🔑 Default Admin Account
On first run, the system automatically creates a default admin account:

Field	Value
Username	admin
Password	admin123
Role	Administrator
⚠️ It is recommended to change the password after first login.

🚀 How to Use
1. As an Admin
Login with default admin credentials

Approve pending seller registrations

Manage users and products

Set global shipping charge

View sales reports

2. As a Seller
Register as a seller

Wait for admin approval

Login after approval

Add products to inventory

Manage orders and update status

3. As a Buyer
Register as a buyer

Login to your account

Browse and search products

Add items to cart

Checkout and view order history

📁 Project Structure
text
ecom/
├── main.c          # Program entry point & main menu
├── functions.h     # Header file with all declarations
├── account.c       # Authentication & user management
├── admin.c         # Admin dashboard & controls
├── seller.c        # Seller dashboard & product management
├── buyer.c         # Buyer dashboard & shopping features
├── product.c       # Product CRUD operations
├── cart.c          # Shopping cart management
├── order.c         # Order processing & invoices
├── report.c        # Sales & revenue reporting
├── utility.c       # Helper functions & utilities
├── README.md       # Documentation
└── data/           # Data storage files (auto-generated)
    ├── accounts.txt    # User account data
    ├── products.txt    # Product inventory
    ├── orders.txt      # Order records
    ├── cart.txt        # Shopping cart data
    └── sales.txt       # Sales reports
💾 Data Files
All data is stored in plain text files in the same directory as the executable:

File	Description
accounts.txt	Stores user account information
products.txt	Stores product inventory data
orders.txt	Stores all order records
cart.txt	Temporary shopping cart data
sales.txt	Exported sales reports
These files are automatically created on the first run.

🖥️ Screenshots
Main Menu
text
================================================
        E-COMMERCE MANAGEMENT SYSTEM
================================================

           1. Login
           2. Register
           3. Exit

           Enter your choice: 
Admin Dashboard
text
================================================
        ADMIN DASHBOARD
================================================

Welcome, System Administrator (Administrator)

1. Approve Sellers
2. Manage Users
3. Product Management
4. View Sales Report
5. View All Orders
6. Manage Shipping Charge
7. Change Password
8. Logout

Enter your choice:
Shopping Cart
text
================================================
        SHOPPING CART
================================================

Product Name             Price      Qty    Subtotal
------------------------------------------------------------
iPhone 15 Pro            BDT 120000.00 1     BDT 120000.00
Samsung Galaxy S24       BDT 100000.00 2     BDT 200000.00
------------------------------------------------------------
Subtotal: BDT 320000.00
Shipping Charge (Flat per order): BDT 50.00
Total: BDT 320050.00
================================================

1. Checkout
2. Remove Item
3. Update Quantity
4. Clear Cart
5. Back to Dashboard


