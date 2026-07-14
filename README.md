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

