#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <windows.h>

// Structure for product
struct Product {
    int id;
    char name[50];
    int quantity;
    float price;
    char category[30];
};

// Structure for user accounts
struct User {
    char username[50];
    char password[100]; // Encrypted password will be stored
    char role[20]; // "admin" or "user"
};

// Structure for sales record
struct Sales {
    int product_id;
    char product_name[50];
    int quantity_sold;
    float total_price;
    char date[20];
};

// Function prototypes
void loginScreen();
void adminMenu();
void userMenu();
void addProduct();
void viewProducts();
void searchProduct();
void updateProduct();
void deleteProduct();
void lowStockAlert();
void generateSalesReport();
void recordSale();
void addUser();
void viewUsers();
void encryptPassword(char* password);
int verifyPassword(char* inputPassword, char* storedPassword);
void changePassword();

// Global variables
char currentUser[50] = "";
char currentUserRole[20] = "";

// Simple XOR encryption key
const char encryptionKey[] = "InventoryKey123";

// Main function
int main() {
    system("cls");
    system("color 3F"); // Blue background, white text
    
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t************************************************\n");
    printf("\t\t\t\t\t\t\t================================================\n");
    printf("\t\t\t\t\t\t\t||                                             ||\n");
    printf("\t\t\t\t\t\t\t||       ------------------------------        ||\n");
    printf("\t\t\t\t\t\t\t||      **INVENTORY MANAGEMENT SYSTEM**        ||\n");
    printf("\t\t\t\t\t\t\t||      ------------------------------         ||\n");
    printf("\t\t\t\t\t\t\t||                                             ||\n");
    printf("\t\t\t\t\t\t\t||                                             ||\n");
    printf("\t\t\t\t\t\t\t||                                             ||\n");
    printf("\t\t\t\t\t\t\t================================================\n");
    printf("\t\t\t\t\t\t\t************************************************\n\n\n");
    printf(" \n\n\n\t\t\t\t\t\t\t Press any key to continue...");
    getch();
    
    loginScreen();
    return 0;
}

// Login screen function
void loginScreen() {
    char username[50];
    char password[50];
    char encryptedPassword[100];
    int found = 0;
    
    struct User user;
    
    FILE *fp = fopen("users.dat", "rb");
    if (fp == NULL) {
        printf("\n\n\t\t\t\t\t\t\tNo users registered. Creating admin account...");
        strcpy(user.username, "admin");
        strcpy(user.password, "admin123");
        encryptPassword(user.password);
        strcpy(user.role, "admin");
        
        fp = fopen("users.dat", "wb");
        fwrite(&user, sizeof(user), 1, fp);
        fclose(fp);
        
        printf("\n\n\t\t\t\t\t\t\tDefault admin account created (username: admin, password: admin123)");
        printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
        getch();
        loginScreen();
        return;
    }
    
    system("cls");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t************************************************\n");
    printf("\t\t\t\t\t\t\t\t\tLOGIN PAGE\n");
    printf("\t\t\t\t\t\t\t************************************************\n");
    
    printf("\n\n\t\t\t\t\t\t\tEnter Username: ");
    scanf("%s", username);
    
    printf("\n\t\t\t\t\t\t\tEnter Password: ");
    
    // Password masking
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == 13) { // Enter key
            password[i] = '\0';
            break;
        } else if (ch == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i] = ch;
            i++;
            printf("*");
        }
    }
    
    rewind(fp);
    while (fread(&user, sizeof(user), 1, fp) == 1) {
        if (strcmp(user.username, username) == 0) {
            found = 1;
            if (verifyPassword(password, user.password)) {
                strcpy(currentUser, username);
                strcpy(currentUserRole, user.role);
                printf("\n\n\n\t\t\t\t\t\t\tLogin successful! Welcome %s (%s).", username, user.role);
                printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
                getch();
                
                if (strcmp(user.role, "admin") == 0) {
                    adminMenu();
                } else {
                    userMenu();
                }
            } else {
                printf("\n\n\n\t\t\t\t\t\t\tInvalid password. Please try again.");
                printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
                getch();
                loginScreen();
            }
            break;
        }
    }
    
    if (!found) {
        printf("\n\n\n\t\t\t\t\t\t\tUsername not found. Please try again.");
        printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
        getch();
        loginScreen();
    }
    
    fclose(fp);
}

// Admin menu function
void adminMenu() {
    int choice;
    
    do {
        system("cls");
        printf("\n\n\n\t\t\t\t\t\t\t================================================");
        printf("\n\t\t\t\t\t\t\t ** INVENTORY MANAGEMENT SYSTEM - ADMIN PANEL **");
        printf("\n\t\t\t\t\t\t\t================================================\n");
        printf("\n\t\t\t\t\t\t\tLogged in as: %s (%s)", currentUser, currentUserRole);
        printf("\n\t\t\t\t\t\t\t================================================\n");
        
        printf("\n\n\t\t\t\t\t\t\t1. Add Product");
        printf("\n\t\t\t\t\t\t\t2. View All Products");
        printf("\n\t\t\t\t\t\t\t3. Search Product");
        printf("\n\t\t\t\t\t\t\t4. Update Product");
        printf("\n\t\t\t\t\t\t\t5. Delete Product");
        printf("\n\t\t\t\t\t\t\t6. Low Stock Alert");
        printf("\n\t\t\t\t\t\t\t7. Record Sale");
        printf("\n\t\t\t\t\t\t\t8. Generate Sales Report");
        printf("\n\t\t\t\t\t\t\t9. Add User Account");
        printf("\n\t\t\t\t\t\t\t10. View User Accounts");
        printf("\n\t\t\t\t\t\t\t11. Change Password");
        printf("\n\t\t\t\t\t\t\t0. Logout");
        printf("\n\n\t\t\t\t\t\t\tEnter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: addProduct(); break;
            case 2: viewProducts(); break;
            case 3: searchProduct(); break;
            case 4: updateProduct(); break;
            case 5: deleteProduct(); break;
            case 6: lowStockAlert(); break;
            case 7: recordSale(); break;
            case 8: generateSalesReport(); break;
            case 9: addUser(); break;
            case 10: viewUsers(); break;
            case 11: changePassword(); break;
            case 0: 
                strcpy(currentUser, "");
                strcpy(currentUserRole, "");
                loginScreen();
                return;
            default:
                printf("\n\n\t\t\t\t\t\t\tInvalid choice. Please try again.");
                printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
                getch();
        }
    } while (choice != 0);
}

// User menu function
void userMenu() {
    int choice;
    
    do {
        system("cls");
        printf("\n\n\n\t\t\t\t\t\t\t================================================");
        printf("\n\t\t\t\t\t\t\t ** INVENTORY MANAGEMENT SYSTEM - USER PANEL **");
        printf("\n\t\t\t\t\t\t\t================================================\n");
        printf("\n\t\t\t\t\t\t\tLogged in as: %s (%s)", currentUser, currentUserRole);
        printf("\n\t\t\t\t\t\t\t================================================\n");
        
        printf("\n\n\t\t\t\t\t\t\t1. View All Products");
        printf("\n\t\t\t\t\t\t\t2. Search Product");
        printf("\n\t\t\t\t\t\t\t3. Low Stock Alert");
        printf("\n\t\t\t\t\t\t\t4. Record Sale");
        printf("\n\t\t\t\t\t\t\t5. Change Password");
        printf("\n\t\t\t\t\t\t\t0. Logout");
        printf("\n\n\t\t\t\t\t\t\tEnter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: viewProducts(); break;
            case 2: searchProduct(); break;
            case 3: lowStockAlert(); break;
            case 4: recordSale(); break;
            case 5: changePassword(); break;
            case 0: 
                strcpy(currentUser, "");
                strcpy(currentUserRole, "");
                loginScreen();
                return;
            default:
                printf("\n\n\t\t\t\t\t\t\tInvalid choice. Please try again.");
                printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
                getch();
        }
    } while (choice != 0);
}

// Add product function
void addProduct() {
    struct Product product;
    FILE *fp;
    char another = 'y';
    
    system("cls");
    printf("\n\n\n\t\t\t\t\t\t\t================================================");
    printf("\n\t\t\t\t\t\t\t\t\tADD PRODUCT");
    printf("\n\t\t\t\t\t\t\t================================================\n");
    
    fp = fopen("products.dat", "ab+");
    if (fp == NULL) {
        printf("\n\n\t\t\t\t\t\t\tError opening file!");
        return;
    }
    
    fflush(stdin);
    while (another == 'y' || another == 'Y') {
        printf("\n\n\t\t\t\t\t\t\tEnter Product ID: ");
        scanf("%d", &product.id);
        
        // Check if product ID already exists
        struct Product temp;
        int idExists = 0;
        rewind(fp);
        while (fread(&temp, sizeof(temp), 1, fp) == 1) {
            if (temp.id == product.id) {
                idExists = 1;
                break;
            }
        }
        
        if (idExists) {
            printf("\n\n\t\t\t\t\t\t\tProduct ID already exists. Please try again.");
            printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
            getch();
            continue;
        }
        
        fflush(stdin);
        printf("\t\t\t\t\t\t\tEnter Product Name: ");
        gets(product.name);
        
        printf("\t\t\t\t\t\t\tEnter Quantity: ");
        scanf("%d", &product.quantity);
        
        printf("\t\t\t\t\t\t\tEnter Price: ");
        scanf("%f", &product.price);
        
        fflush(stdin);
        printf("\t\t\t\t\t\t\tEnter Category: ");
        gets(product.category);
        
        fwrite(&product, sizeof(product), 1, fp);
        
        printf("\n\n\t\t\t\t\t\t\tProduct added successfully!");
        printf("\n\n\t\t\t\t\t\t\tAdd another product? (y/n): ");
        fflush(stdin);
        another = getche();
    }
    
    fclose(fp);
    printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
    getch();
}

// View products function
void viewProducts() {
    struct Product product;
    FILE *fp;
    int count = 0;
    
    system("cls");
    printf("\n\n\n\t\t\t\t\t\t\t================================================");
    printf("\n\t\t\t\t\t\t\t\t\tALL PRODUCTS");
    printf("\n\t\t\t\t\t\t\t================================================\n");
    
    printf("\n\n\t\t\t\t\t\t\t%-10s %-20s %-10s %-10s %-20s", "ID", "Name", "Quantity", "Price", "Category");
    printf("\n\t\t\t\t\t\t\t------------------------------------------------------------");
    
    fp = fopen("products.dat", "rb");
    if (fp == NULL) {
        printf("\n\n\t\t\t\t\t\t\tNo products found!");
        printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
        getch();
        return;
    }
    
    while (fread(&product, sizeof(product), 1, fp) == 1) {
        printf("\n\t\t\t\t\t\t\t%-10d %-20s %-10d $%-9.2f %-20s", 
               product.id, product.name, product.quantity, product.price, product.category);
        count++;
    }
    
    if (count == 0) {
        printf("\n\n\t\t\t\t\t\t\tNo products found in inventory!");
    } else {
        printf("\n\n\t\t\t\t\t\t\tTotal products: %d", count);
    }
    
    fclose(fp);
    printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
    getch();
}

// Search product function
void searchProduct() {
    struct Product product;
    FILE *fp;
    int choice, found = 0;
    int searchId;
    char searchName[50];
    
    system("cls");
    printf("\n\n\n\t\t\t\t\t\t\t================================================");
    printf("\n\t\t\t\t\t\t\t\t\tSEARCH PRODUCT");
    printf("\n\t\t\t\t\t\t\t================================================\n");
    
    printf("\n\n\t\t\t\t\t\t\t1. Search by ID");
    printf("\n\t\t\t\t\t\t\t2. Search by Name");
    printf("\n\t\t\t\t\t\t\t0. Back to Menu");
    printf("\n\n\t\t\t\t\t\t\tEnter your choice: ");
    scanf("%d", &choice);
    
    fp = fopen("products.dat", "rb");
    if (fp == NULL) {
        printf("\n\n\t\t\t\t\t\t\tNo products found!");
        printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
        getch();
        return;
    }
    
    switch(choice) {
        case 1:
            printf("\n\n\t\t\t\t\t\t\tEnter Product ID to search: ");
            scanf("%d", &searchId);
            
            system("cls");
            printf("\n\n\n\t\t\t\t\t\t\t================================================");
            printf("\n\t\t\t\t\t\t\t\tSEARCH RESULTS FOR ID: %d", searchId);
            printf("\n\t\t\t\t\t\t\t================================================\n");
            
            printf("\n\n\t\t\t\t\t\t\t%-10s %-20s %-10s %-10s %-20s", "ID", "Name", "Quantity", "Price", "Category");
            printf("\n\t\t\t\t\t\t\t------------------------------------------------------------");
            
            while (fread(&product, sizeof(product), 1, fp) == 1) {
                if (product.id == searchId) {
                    printf("\n\t\t\t\t\t\t\t%-10d %-20s %-10d $%-9.2f %-20s", 
                           product.id, product.name, product.quantity, product.price, product.category);
                    found = 1;
                    break;
                }
            }
            break;
            
        case 2:
            printf("\n\n\t\t\t\t\t\t\tEnter Product Name to search: ");
            fflush(stdin);
            gets(searchName);
            
            system("cls");
            printf("\n\n\n\t\t\t\t\t\t\t================================================");
            printf("\n\t\t\t\t\t\t\t\tSEARCH RESULTS FOR: %s", searchName);
            printf("\n\t\t\t\t\t\t\t================================================\n");
            
            printf("\n\n\t\t\t\t\t\t\t%-10s %-20s %-10s %-10s %-20s", "ID", "Name", "Quantity", "Price", "Category");
            printf("\n\t\t\t\t\t\t\t------------------------------------------------------------");
            
            while (fread(&product, sizeof(product), 1, fp) == 1) {
                if (strstr(product.name, searchName) != NULL) {
                    printf("\n\t\t\t\t\t\t\t%-10d %-20s %-10d $%-9.2f %-20s", 
                           product.id, product.name, product.quantity, product.price, product.category);
                    found++;
                }
            }
            break;
            
        case 0:
            fclose(fp);
            return;
            
        default:
            printf("\n\n\t\t\t\t\t\t\tInvalid choice!");
            printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
            getch();
            fclose(fp);
            return;
    }
    
    if (!found) {
        printf("\n\n\t\t\t\t\t\t\tNo matching products found!");
    } else if (choice == 2) {
        printf("\n\n\t\t\t\t\t\t\tTotal matches: %d", found);
    }
    
    fclose(fp);
    printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
    getch();
}

// Update product function
void updateProduct() {
    struct Product product;
    FILE *fp, *temp;
    int id, found = 0;
    
    system("cls");
    printf("\n\n\n\t\t\t\t\t\t\t================================================");
    printf("\n\t\t\t\t\t\t\t\t\tUPDATE PRODUCT");
    printf("\n\t\t\t\t\t\t\t================================================\n");
    
    printf("\n\n\t\t\t\t\t\t\tEnter Product ID to update: ");
    scanf("%d", &id);
    
    fp = fopen("products.dat", "rb");
    temp = fopen("temp.dat", "wb");
    
    if (fp == NULL || temp == NULL) {
        printf("\n\n\t\t\t\t\t\t\tError opening files!");
        printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
        getch();
        return;
    }
    
    while (fread(&product, sizeof(product), 1, fp) == 1) {
        if (product.id == id) {
            found = 1;
            printf("\n\n\t\t\t\t\t\t\tCurrent Product Details:");
            printf("\n\t\t\t\t\t\t\t------------------------");
            printf("\n\t\t\t\t\t\t\tID: %d", product.id);
            printf("\n\t\t\t\t\t\t\tName: %s", product.name);
            printf("\n\t\t\t\t\t\t\tQuantity: %d", product.quantity);
            printf("\n\t\t\t\t\t\t\tPrice: $%.2f", product.price);
            printf("\n\t\t\t\t\t\t\tCategory: %s", product.category);
            
            printf("\n\n\t\t\t\t\t\t\tEnter New Details:");
            printf("\n\t\t\t\t\t\t\t------------------");
            
            printf("\n\t\t\t\t\t\t\tEnter Product Name: ");
            fflush(stdin);
            gets(product.name);
            
            printf("\t\t\t\t\t\t\tEnter Quantity: ");
            scanf("%d", &product.quantity);
            
            printf("\t\t\t\t\t\t\tEnter Price: ");
            scanf("%f", &product.price);
            
            fflush(stdin);
            printf("\t\t\t\t\t\t\tEnter Category: ");
            gets(product.category);
            
            printf("\n\n\t\t\t\t\t\t\tProduct updated successfully!");
        }
        fwrite(&product, sizeof(product), 1, temp);
    }
    
    fclose(fp);
    fclose(temp);
    
    if (!found) {
        printf("\n\n\t\t\t\t\t\t\tProduct with ID %d not found!", id);
        remove("temp.dat");
    } else {
        remove("products.dat");
        rename("temp.dat", "products.dat");
    }
    
    printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
    getch();
}

// Delete product function
void deleteProduct() {
    struct Product product;
    FILE *fp, *temp;
    int id, found = 0;
    char confirm;
    
    system("cls");
    printf("\n\n\n\t\t\t\t\t\t\t================================================");
    printf("\n\t\t\t\t\t\t\t\t\tDELETE PRODUCT");
    printf("\n\t\t\t\t\t\t\t================================================\n");
    
    printf("\n\n\t\t\t\t\t\t\tEnter Product ID to delete: ");
    scanf("%d", &id);
    
    fp = fopen("products.dat", "rb");
    if (fp == NULL) {
        printf("\n\n\t\t\t\t\t\t\tNo products found!");
        printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
        getch();
        return;
    }
    
    // First display the product to confirm deletion
    while (fread(&product, sizeof(product), 1, fp) == 1) {
        if (product.id == id) {
            found = 1;
            printf("\n\n\t\t\t\t\t\t\tProduct Details:");
            printf("\n\t\t\t\t\t\t\t----------------");
            printf("\n\t\t\t\t\t\t\tID: %d", product.id);
            printf("\n\t\t\t\t\t\t\tName: %s", product.name);
            printf("\n\t\t\t\t\t\t\tQuantity: %d", product.quantity);
            printf("\n\t\t\t\t\t\t\tPrice: $%.2f", product.price);
            printf("\n\t\t\t\t\t\t\tCategory: %s", product.category);
            
            printf("\n\n\t\t\t\t\t\t\tAre you sure you want to delete this product? (y/n): ");
            fflush(stdin);
            confirm = getche();
            
            if (confirm == 'y' || confirm == 'Y') {
                rewind(fp);
                temp = fopen("temp.dat", "wb");
                
                while (fread(&product, sizeof(product), 1, fp) == 1) {
                    if (product.id != id) {
                        fwrite(&product, sizeof(product), 1, temp);
                    }
                }
                
                fclose(fp);
                fclose(temp);
                
                remove("products.dat");
                rename("temp.dat", "products.dat");
                
                printf("\n\n\t\t\t\t\t\t\tProduct deleted successfully!");
            } else {
                printf("\n\n\t\t\t\t\t\t\tDeletion cancelled.");
            }
            break;
        }
    }
    
    if (!found) {
        printf("\n\n\t\t\t\t\t\t\tProduct with ID %d not found!", id);
    }
    
    if (fp) fclose(fp);
    printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
    getch();
}

// Low stock alert function
void lowStockAlert() {
    struct Product product;
    FILE *fp;
    int threshold = 10; // Set your low stock threshold here
    int count = 0;
    
    system("cls");
    printf("\n\n\n\t\t\t\t\t\t\t================================================");
    printf("\n\t\t\t\t\t\t\t\t\tLOW STOCK ALERT");
    printf("\n\t\t\t\t\t\t\t================================================\n");
    printf("\n\n\t\t\t\t\t\t\tProducts with quantity less than %d:", threshold);
    printf("\n\n\t\t\t\t\t\t\t%-10s %-20s %-10s %-10s %-20s", "ID", "Name", "Quantity", "Price", "Category");
    printf("\n\t\t\t\t\t\t\t------------------------------------------------------------");
    
    fp = fopen("products.dat", "rb");
    if (fp == NULL) {
        printf("\n\n\t\t\t\t\t\t\tNo products found!");
        printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
        getch();
        return;
    }
    
    while (fread(&product, sizeof(product), 1, fp) == 1) {
        if (product.quantity < threshold) {
            printf("\n\t\t\t\t\t\t\t%-10d %-20s %-10d $%-9.2f %-20s", 
                   product.id, product.name, product.quantity, product.price, product.category);
            count++;
        }
    }
    
    if (count == 0) {
        printf("\n\n\t\t\t\t\t\t\tNo products with low stock!");
    } else {
        printf("\n\n\t\t\t\t\t\t\tTotal low stock items: %d", count);
    }
    
    fclose(fp);
    printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
    getch();
}

// Record sale function
void recordSale() {
    struct Product product;
    struct Sales sale;
    FILE *fp, *salesFile;
    int id, quantity, found = 0;
    time_t t;
    struct tm *tm_info;
    char dateStr[20];
    
    system("cls");
    printf("\n\n\n\t\t\t\t\t\t\t================================================");
    printf("\n\t\t\t\t\t\t\t\t\tRECORD SALE");
    printf("\n\t\t\t\t\t\t\t================================================\n");
    
    printf("\n\n\t\t\t\t\t\t\tEnter Product ID sold: ");
    scanf("%d", &id);
    
    fp = fopen("products.dat", "rb+");
    if (fp == NULL) {
        printf("\n\n\t\t\t\t\t\t\tNo products found!");
        printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
        getch();
        return;
    }
    
    // Find the product
    while (fread(&product, sizeof(product), 1, fp) == 1) {
        if (product.id == id) {
            found = 1;
            printf("\n\n\t\t\t\t\t\t\tProduct: %s", product.name);
            printf("\n\t\t\t\t\t\t\tCurrent Stock: %d", product.quantity);
            printf("\n\t\t\t\t\t\t\tPrice per unit: $%.2f", product.price);
            
            printf("\n\n\t\t\t\t\t\t\tEnter quantity sold: ");
            scanf("%d", &quantity);
            
            if (quantity > product.quantity) {
                printf("\n\n\t\t\t\t\t\t\tNot enough stock available!");
                printf("\n\t\t\t\t\t\t\tAvailable stock: %d", product.quantity);
            } else {
                // Update product quantity
                product.quantity -= quantity;
                fseek(fp, -sizeof(product), SEEK_CUR);
                fwrite(&product, sizeof(product), 1, fp);
                
                // Record the sale
                time(&t);
                tm_info = localtime(&t);
                strftime(dateStr, 20, "%Y-%m-%d", tm_info);
                
                strcpy(sale.product_name, product.name);
                sale.product_id = product.id;
                sale.quantity_sold = quantity;
                sale.total_price = quantity * product.price;
                strcpy(sale.date, dateStr);
                
                salesFile = fopen("sales.dat", "ab");
                fwrite(&sale, sizeof(sale), 1, salesFile);
                fclose(salesFile);
                
                printf("\n\n\t\t\t\t\t\t\tSale recorded successfully!");
                printf("\n\t\t\t\t\t\t\tTotal: $%.2f", sale.total_price);
            }
            break;
        }
    }
    
    if (!found) {
        printf("\n\n\t\t\t\t\t\t\tProduct with ID %d not found!", id);
    }
    
    fclose(fp);
    printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
    getch();
}

// Generate sales report function
void generateSalesReport() {
    struct Sales sale;
    FILE *fp;
    float totalSales = 0;
    int count = 0;
    char startDate[20], endDate[20];
    int dateFilter = 0;
    
    system("cls");
    printf("\n\n\n\t\t\t\t\t\t\t================================================");
    printf("\n\t\t\t\t\t\t\t\t\tSALES REPORT");
    printf("\n\t\t\t\t\t\t\t================================================\n");
    
    printf("\n\n\t\t\t\t\t\t\t1. All sales");
    printf("\n\t\t\t\t\t\t\t2. Sales between dates");
    printf("\n\t\t\t\t\t\t\tEnter your choice: ");
    scanf("%d", &dateFilter);
    
    if (dateFilter == 2) {
        printf("\n\n\t\t\t\t\t\t\tEnter start date (YYYY-MM-DD): ");
        scanf("%s", startDate);
        printf("\t\t\t\t\t\t\tEnter end date (YYYY-MM-DD): ");
        scanf("%s", endDate);
    }
    
    printf("\n\n\t\t\t\t\t\t\t%-10s %-20s %-10s %-10s %-15s", "ID", "Product", "Qty", "Total", "Date");
    printf("\n\t\t\t\t\t\t\t------------------------------------------------------------");
    
    fp = fopen("sales.dat", "rb");
    if (fp == NULL) {
        printf("\n\n\t\t\t\t\t\t\tNo sales records found!");
        printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
        getch();
        return;
    }
    
    while (fread(&sale, sizeof(sale), 1, fp) == 1) {
        if (dateFilter == 1 || 
            (strcmp(sale.date, startDate) >= 0 && strcmp(sale.date, endDate) <= 0)) {
            printf("\n\t\t\t\t\t\t\t%-10d %-20s %-10d $%-9.2f %-15s", 
                   sale.product_id, sale.product_name, sale.quantity_sold, sale.total_price, sale.date);
            totalSales += sale.total_price;
            count++;
        }
    }
    
    if (count == 0) {
        printf("\n\n\t\t\t\t\t\t\tNo sales records found!");
    } else {
        printf("\n\n\t\t\t\t\t\t\tTotal sales: %d transactions", count);
        printf("\n\t\t\t\t\t\t\tTotal revenue: $%.2f", totalSales);
    }
    
    fclose(fp);
    printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
    getch();
}

// Add user function
void addUser() {
    struct User user;
    FILE *fp;
    char another = 'y';
    char password[50];
    
    system("cls");
    printf("\n\n\n\t\t\t\t\t\t\t================================================");
    printf("\n\t\t\t\t\t\t\t\t\tADD USER");
    printf("\n\t\t\t\t\t\t\t================================================\n");
    
    fp = fopen("users.dat", "ab+");
    if (fp == NULL) {
        printf("\n\n\t\t\t\t\t\t\tError opening file!");
        return;
    }
    
    fflush(stdin);
    while (another == 'y' || another == 'Y') {
        printf("\n\n\t\t\t\t\t\t\tEnter Username: ");
        scanf("%s", user.username);
        
        // Check if username already exists
        struct User temp;
        int userExists = 0;
        rewind(fp);
        while (fread(&temp, sizeof(temp), 1, fp) == 1) {
            if (strcmp(temp.username, user.username) == 0) {
                userExists = 1;
                break;
            }
        }
        
        if (userExists) {
            printf("\n\n\t\t\t\t\t\t\tUsername already exists. Please try again.");
            printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
            getch();
            continue;
        }
        
        printf("\t\t\t\t\t\t\tEnter Password: ");
        // Password masking
        int i = 0;
        char ch;
        while (1) {
            ch = getch();
            if (ch == 13) { // Enter key
                password[i] = '\0';
                break;
            } else if (ch == 8) { // Backspace
                if (i > 0) {
                    i--;
                    printf("\b \b");
                }
            } else {
                password[i] = ch;
                i++;
                printf("*");
            }
        }
        
        strcpy(user.password, password);
        encryptPassword(user.password);
        
        printf("\n\t\t\t\t\t\t\tEnter Role (admin/user): ");
        scanf("%s", user.role);
        
        // Validate role
        if (strcmp(user.role, "admin") != 0 && strcmp(user.role, "user") != 0) {
            printf("\n\n\t\t\t\t\t\t\tInvalid role. Only 'admin' or 'user' allowed.");
            printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
            getch();
            continue;
        }
        
        fwrite(&user, sizeof(user), 1, fp);
        
        printf("\n\n\t\t\t\t\t\t\tUser added successfully!");
        printf("\n\n\t\t\t\t\t\t\tAdd another user? (y/n): ");
        fflush(stdin);
        another = getche();
    }
    
    fclose(fp);
    printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
    getch();
}

// View users function
void viewUsers() {
    struct User user;
    FILE *fp;
    int count = 0;
    
    system("cls");
    printf("\n\n\n\t\t\t\t\t\t\t================================================");
    printf("\n\t\t\t\t\t\t\t\t\tALL USERS");
    printf("\n\t\t\t\t\t\t\t================================================\n");
    
    printf("\n\n\t\t\t\t\t\t\t%-20s %-10s", "Username", "Role");
    printf("\n\t\t\t\t\t\t\t------------------------------");
    
    fp = fopen("users.dat", "rb");
    if (fp == NULL) {
        printf("\n\n\t\t\t\t\t\t\tNo users found!");
        printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
        getch();
        return;
    }
    
    while (fread(&user, sizeof(user), 1, fp) == 1) {
        printf("\n\t\t\t\t\t\t\t%-20s %-10s", user.username, user.role);
        count++;
    }
    
    if (count == 0) {
        printf("\n\n\t\t\t\t\t\t\tNo users found!");
    } else {
        printf("\n\n\t\t\t\t\t\t\tTotal users: %d", count);
    }
    
    fclose(fp);
    printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
    getch();
}

// Password encryption function
void encryptPassword(char* password) {
    int len = strlen(password);
    int keyLen = strlen(encryptionKey);
    
    for (int i = 0; i < len; i++) {
        password[i] = password[i] ^ encryptionKey[i % keyLen];
    }
}

// Password verification function
int verifyPassword(char* inputPassword, char* storedPassword) {
    char temp[100];
    strcpy(temp, storedPassword);
    encryptPassword(temp); // Decrypt the stored password (XOR is reversible)
    
    return strcmp(inputPassword, temp) == 0;
}

// Change password function
void changePassword() {
    struct User user, tempUser;
    FILE *fp, *temp;
    int found = 0;
    char currentPassword[50], newPassword[50], confirmPassword[50];
    
    system("cls");
    printf("\n\n\n\t\t\t\t\t\t\t================================================");
    printf("\n\t\t\t\t\t\t\t\t\tCHANGE PASSWORD");
    printf("\n\t\t\t\t\t\t\t================================================\n");
    
        fp = fopen("users.dat", "rb+");
    if (fp == NULL) {
        printf("\n\n\t\t\t\t\t\t\tError opening user database!");
        printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
        getch();
        return;
    }

    // Get current password
    printf("\n\n\t\t\t\t\t\t\tEnter current password: ");
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == 13) { // Enter key
            currentPassword[i] = '\0';
            break;
        } else if (ch == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            currentPassword[i] = ch;
            i++;
            printf("*");
        }
    }

    // Verify current password
    int passwordCorrect = 0;
    while (fread(&user, sizeof(user), 1, fp) == 1) {
        if (strcmp(user.username, currentUser) == 0) {
            if (verifyPassword(currentPassword, user.password)) {
                passwordCorrect = 1;
                found = 1;
                break;
            }
        }
    }

    if (!passwordCorrect) {
        printf("\n\n\t\t\t\t\t\t\tIncorrect current password!");
        printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
        fclose(fp);
        getch();
        return;
    }

    // Get new password
    printf("\n\n\t\t\t\t\t\t\tEnter new password: ");
    i = 0;
    while (1) {
        ch = getch();
        if (ch == 13) { // Enter key
            newPassword[i] = '\0';
            break;
        } else if (ch == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            newPassword[i] = ch;
            i++;
            printf("*");
        }
    }

    // Confirm new password
    printf("\n\n\t\t\t\t\t\t\tConfirm new password: ");
    i = 0;
    while (1) {
        ch = getch();
        if (ch == 13) { // Enter key
            confirmPassword[i] = '\0';
            break;
        } else if (ch == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            confirmPassword[i] = ch;
            i++;
            printf("*");
        }
    }

    // Check if new passwords match
    if (strcmp(newPassword, confirmPassword) != 0) {
        printf("\n\n\t\t\t\t\t\t\tNew passwords don't match!");
        printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
        fclose(fp);
        getch();
        return;
    }

    // Update password in the file
    rewind(fp);
    temp = fopen("temp.dat", "wb");
    
    while (fread(&tempUser, sizeof(tempUser), 1, fp) == 1) {
        if (strcmp(tempUser.username, currentUser) == 0) {
            strcpy(tempUser.password, newPassword);
            encryptPassword(tempUser.password);
        }
        fwrite(&tempUser, sizeof(tempUser), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("users.dat");
    rename("temp.dat", "users.dat");

    printf("\n\n\t\t\t\t\t\t\tPassword changed successfully!");
    printf("\n\n\t\t\t\t\t\t\tPress any key to continue...");
    getch();
}

// Additional helper functions would go here if needed