#include "functions.h"

char* generateId(const char* prefix) {
    static char id[MAX_ID];
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    
    sprintf(id, "%s%02d%02d%02d%02d%02d%02d", 
            prefix,
            tm_info->tm_year % 100,
            tm_info->tm_mon + 1,
            tm_info->tm_mday,
            tm_info->tm_hour,
            tm_info->tm_min,
            tm_info->tm_sec);
    
    return id;
}

char* getCurrentDateTime() {
    static char dateTime[50];
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    
    strftime(dateTime, sizeof(dateTime), "%Y-%m-%d %H:%M:%S", tm_info);
    return dateTime;
}

int validateEmail(const char* email) {
    if (email == NULL) return 0;
    
    char* atPos = strchr(email, '@');
    char* dotPos = strchr(email, '.');
    
    if (atPos == NULL || dotPos == NULL) return 0;
    if (atPos > dotPos) return 0;
    if (strlen(email) < 5) return 0;
    
    for (int i = 0; email[i] != '\0'; i++) {
        if (!isalnum(email[i]) && email[i] != '@' && email[i] != '.' && email[i] != '-' && email[i] != '_') {
            return 0;
        }
    }
    
    return 1;
}

int validatePhone(const char* phone) {
    if (phone == NULL) return 0;
    
    int len = strlen(phone);
    if (len < 10 || len > 15) return 0;
    
    for (int i = 0; i < len; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    
    return 1;
}

int validatePassword(const char* password) {
    if (password == NULL) return 0;
    
    int len = strlen(password);
    if (len < 6 || len > MAX_PASSWORD) return 0;
    
    int hasUpper = 0, hasLower = 0, hasDigit = 0;
    
    for (int i = 0; i < len; i++) {
        if (isupper(password[i])) hasUpper = 1;
        if (islower(password[i])) hasLower = 1;
        if (isdigit(password[i])) hasDigit = 1;
    }
    
    return (hasUpper && hasLower && hasDigit);
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseScreen() {
    printf("\n\t\t\t\t\tPress Enter to continue...");
    getchar();
}

int confirmAction(const char* message) {
    char response;
    printf("\n\t\t\t\t\t%s (y/n): ", message);
    scanf(" %c", &response);
    getchar();
    return (response == 'y' || response == 'Y');
}

int fileExists(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp) {
        fclose(fp);
        return 1;
    }
    return 0;
}

void trimString(char* str) {
    if (str == NULL) return;
    
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n') {
        start++;
    }
    
    if (start > 0) {
        int i;
        for (i = 0; str[start + i] != '\0'; i++) {
            str[i] = str[start + i];
        }
        str[i] = '\0';
    }
    
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\n')) {
        len--;
    }
    str[len] = '\0';
}

void centerText(const char* text) {
    if (text == NULL) return;
    
    int width = 80;
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        }
    #endif
    
    int textLen = strlen(text);
    int padding = (width - textLen) / 2;
    
    if (padding < 0) padding = 0;
    
    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s\n", text);
}

void centerTitle(const char* title) {
    printf("\n");
    centerText("================================================");
    centerText(title);
    centerText("================================================");
    printf("\n");
}

void centerMenu(const char* options[], int count) {
    for (int i = 0; i < count; i++) {
        centerText(options[i]);
    }
}

int getIntegerInput(const char* prompt) {
    int value;
    printf("\n\t\t\t\t\t%s", prompt);
    scanf("%d", &value);
    getchar();
    return value;
}

double getDoubleInput(const char* prompt) {
    double value;
    printf("\n\t\t\t\t\t%s", prompt);
    scanf("%lf", &value);
    getchar();
    return value;
}

void getStringInput(const char* prompt, char* buffer, int size) {
    printf("\n\t\t\t\t\t%s", prompt);
    fgets(buffer, size, stdin);
    trimString(buffer);
}