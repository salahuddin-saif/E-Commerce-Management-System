#include "functions.h"

int login() {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    
    clearScreen();
    centerTitle("LOGIN");
    
    printf("\n");
    getStringInput("Username: ", username, sizeof(username));
    getStringInput("Password: ", password, sizeof(password));
    
    if (strlen(username) == 0 || strlen(password) == 0) {
        centerText("Username and password cannot be empty!");
        pauseScreen();
        return 0;
    }
    
    User* user = findUserByUsername(username);
    if (user == NULL) {
        centerText("User not found!");
        pauseScreen();
        return 0;
    }
    
    if (!user->isActive) {
        centerText("Account is deactivated!");
        pauseScreen();
        free(user);
        return 0;
    }
    
    if (user->role == ROLE_SELLER && user->status != STATUS_APPROVED) {
        if (user->status == STATUS_PENDING) {
            centerText("Your seller account is pending approval from admin.");
        } else if (user->status == STATUS_REJECTED) {
            centerText("Your seller account has been rejected.");
        }
        pauseScreen();
        free(user);
        return 0;
    }
    
    if (strcmp(user->password, password) != 0) {
        centerText("Invalid password!");
        pauseScreen();
        free(user);
        return 0;
    }
    
    currentUser = *user;
    currentUserRole = user->role;
    free(user);
    
    char msg[100];
    sprintf(msg, "Login successful! Welcome, %s!", currentUser.fullName);
    centerText(msg);
    pauseScreen();
    return 1;
}

void logout() {
    memset(&currentUser, 0, sizeof(User));
    currentUserRole = 0;
    centerText("Logged out successfully!");
    pauseScreen();
}

int registerUser() {
    User newUser;
    char passwordConfirm[MAX_PASSWORD];
    
    clearScreen();
    centerTitle("REGISTRATION");
    
    printf("\n");
    getStringInput("Username: ", newUser.username, sizeof(newUser.username));
    
    if (isUsernameTaken(newUser.username)) {
        centerText("Username already taken!");
        pauseScreen();
        return 0;
    }
    
    getStringInput("Full Name: ", newUser.fullName, sizeof(newUser.fullName));
    getStringInput("Email: ", newUser.email, sizeof(newUser.email));
    
    if (!validateEmail(newUser.email)) {
        centerText("Invalid email format!");
        pauseScreen();
        return 0;
    }
    
    getStringInput("Phone: ", newUser.phone, sizeof(newUser.phone));
    
    if (!validatePhone(newUser.phone)) {
        centerText("Invalid phone number (must be 10-15 digits)!");
        pauseScreen();
        return 0;
    }
    
    getStringInput("Address: ", newUser.address, sizeof(newUser.address));
    getStringInput("Password: ", newUser.password, sizeof(newUser.password));
    
    if (!validatePassword(newUser.password)) {
        centerText("Password must be at least 6 characters with uppercase, lowercase, and digits!");
        pauseScreen();
        return 0;
    }
    
    getStringInput("Confirm Password: ", passwordConfirm, sizeof(passwordConfirm));
    
    if (strcmp(newUser.password, passwordConfirm) != 0) {
        centerText("Passwords do not match!");
        pauseScreen();
        return 0;
    }
    
    centerText("Select Role:");
    centerText("1. Buyer");
    centerText("2. Seller");
    int roleChoice = getIntegerInput("Enter choice: ");
    
    if (roleChoice == 2) {
        newUser.role = ROLE_SELLER;
        newUser.status = STATUS_PENDING;
    } else {
        newUser.role = ROLE_BUYER;
        newUser.status = STATUS_APPROVED;
    }
    
    strcpy(newUser.id, generateId("USR"));
    newUser.isActive = 1;
    newUser.createdAt = time(NULL);
    
    if (saveUser(&newUser)) {
        centerText("Registration successful!");
        if (newUser.role == ROLE_SELLER) {
            centerText("Your seller account is pending admin approval.");
            centerText("You will be notified when approved.");
        } else {
            centerText("You can now login to your account.");
        }
        pauseScreen();
        return 1;
    } else {
        centerText("Registration failed!");
        pauseScreen();
        return 0;
    }
}

int changePassword() {
    char oldPassword[MAX_PASSWORD];
    char newPassword[MAX_PASSWORD];
    char confirmPassword[MAX_PASSWORD];
    
    clearScreen();
    centerTitle("CHANGE PASSWORD");
    
    printf("\n");
    getStringInput("Enter current password: ", oldPassword, sizeof(oldPassword));
    
    if (strcmp(currentUser.password, oldPassword) != 0) {
        centerText("Current password is incorrect!");
        pauseScreen();
        return 0;
    }
    
    getStringInput("Enter new password: ", newPassword, sizeof(newPassword));
    
    if (!validatePassword(newPassword)) {
        centerText("Password must be at least 6 characters with uppercase, lowercase, and digits!");
        pauseScreen();
        return 0;
    }
    
    getStringInput("Confirm new password: ", confirmPassword, sizeof(confirmPassword));
    
    if (strcmp(newPassword, confirmPassword) != 0) {
        centerText("Passwords do not match!");
        pauseScreen();
        return 0;
    }
    
    strcpy(currentUser.password, newPassword);
    
    if (updateUserInFile(&currentUser)) {
        centerText("Password changed successfully!");
        pauseScreen();
        return 1;
    } else {
        centerText("Failed to change password!");
        pauseScreen();
        return 0;
    }
}

int validateUserCredentials(const char* username, const char* password) {
    User* user = findUserByUsername(username);
    if (user == NULL) return 0;
    
    int valid = (strcmp(user->password, password) == 0);
    free(user);
    return valid;
}

User* findUserByUsername(const char* username) {
    FILE* fp = fopen(ACCOUNTS_FILE, "r");
    if (fp == NULL) return NULL;
    
    User user;
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%d,%ld\n",
                  user.id, user.username, user.password, user.email,
                  user.fullName, user.phone, user.address,
                  &user.role, &user.status, &user.isActive, &user.createdAt) == 11) {
        if (strcmp(user.username, username) == 0) {
            fclose(fp);
            User* foundUser = (User*)malloc(sizeof(User));
            *foundUser = user;
            return foundUser;
        }
    }
    
    fclose(fp);
    return NULL;
}

int saveUser(const User* user) {
    FILE* fp = fopen(ACCOUNTS_FILE, "a");
    if (fp == NULL) return 0;
    
    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s,%d,%d,%d,%ld\n",
            user->id, user->username, user->password, user->email,
            user->fullName, user->phone, user->address,
            user->role, user->status, user->isActive, user->createdAt);
    
    fclose(fp);
    return 1;
}

int loadAllUsers(User** users, int* count) {
    FILE* fp = fopen(ACCOUNTS_FILE, "r");
    if (fp == NULL) {
        *users = NULL;
        *count = 0;
        return 0;
    }
    
    User temp;
    int total = 0;
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%d,%ld\n",
                  temp.id, temp.username, temp.password, temp.email,
                  temp.fullName, temp.phone, temp.address,
                  &temp.role, &temp.status, &temp.isActive, &temp.createdAt) == 11) {
        total++;
    }
    
    if (total == 0) {
        fclose(fp);
        *users = NULL;
        *count = 0;
        return 0;
    }
    
    *users = (User*)malloc(total * sizeof(User));
    if (*users == NULL) {
        fclose(fp);
        return 0;
    }
    
    rewind(fp);
    int i = 0;
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%d,%ld\n",
                  (*users)[i].id, (*users)[i].username, (*users)[i].password, (*users)[i].email,
                  (*users)[i].fullName, (*users)[i].phone, (*users)[i].address,
                  &(*users)[i].role, &(*users)[i].status, &(*users)[i].isActive, &(*users)[i].createdAt) == 11) {
        i++;
    }
    
    fclose(fp);
    *count = total;
    return 1;
}

int updateUserInFile(const User* user) {
    FILE* fp = fopen(ACCOUNTS_FILE, "r");
    if (fp == NULL) return 0;
    
    FILE* temp = fopen("temp_accounts.txt", "w");
    if (temp == NULL) {
        fclose(fp);
        return 0;
    }
    
    User current;
    int found = 0;
    
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%d,%ld\n",
                  current.id, current.username, current.password, current.email,
                  current.fullName, current.phone, current.address,
                  &current.role, &current.status, &current.isActive, &current.createdAt) == 11) {
        
        if (strcmp(current.id, user->id) == 0) {
            fprintf(temp, "%s,%s,%s,%s,%s,%s,%s,%d,%d,%d,%ld\n",
                    user->id, user->username, user->password, user->email,
                    user->fullName, user->phone, user->address,
                    user->role, user->status, user->isActive, user->createdAt);
            found = 1;
        } else {
            fprintf(temp, "%s,%s,%s,%s,%s,%s,%s,%d,%d,%d,%ld\n",
                    current.id, current.username, current.password, current.email,
                    current.fullName, current.phone, current.address,
                    current.role, current.status, current.isActive, current.createdAt);
        }
    }
    
    fclose(fp);
    fclose(temp);
    
    if (found) {
        remove(ACCOUNTS_FILE);
        rename("temp_accounts.txt", ACCOUNTS_FILE);
        return 1;
    } else {
        remove("temp_accounts.txt");
        return 0;
    }
}

int isUsernameTaken(const char* username) {
    User* user = findUserByUsername(username);
    if (user != NULL) {
        free(user);
        return 1;
    }
    return 0;
}