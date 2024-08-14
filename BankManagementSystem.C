#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "customers.txt"

// Define a structure for customer
typedef struct {
    char name[100];
    char accountNumber[20];
    double creditBalance;
} Customer;

// Function prototypes
void addCustomer();
void displayCustomers();
void updateCustomer();
void deleteCustomer();
void loadCustomers();
void saveCustomer(const Customer *customer);
int findCustomerIndex(const char *accountNumber);

int main() {
    int choice;

    // Load existing customers from file
    loadCustomers();

    do {
        printf("\nBank Management System\n");
        printf("1. Add Customer\n");
        printf("2. Display Customers\n");
        printf("3. Update Customer\n");
        printf("4. Delete Customer\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addCustomer();
                break;
            case 2:
                displayCustomers();
                break;
            case 3:
                updateCustomer();
                break;
            case 4:
                deleteCustomer();
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

// Function to add a customer
void addCustomer() {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    Customer customer;
    printf("Enter customer name: ");
    getchar();  // Clear newline character left in buffer
    fgets(customer.name, sizeof(customer.name), stdin);
    customer.name[strcspn(customer.name, "\n")] = '\0';  // Remove newline character

    printf("Enter account number: ");
    fgets(customer.accountNumber, sizeof(customer.accountNumber), stdin);
    customer.accountNumber[strcspn(customer.accountNumber, "\n")] = '\0';  // Remove newline character

    printf("Enter credit balance: ");
    scanf("%lf", &customer.creditBalance);

    saveCustomer(&customer);

    fclose(file);
    printf("Customer added successfully!\n");
}

// Function to display all customers
void displayCustomers() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    Customer customer;
    char line[256];
    printf("\nCustomer Details:\n");
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%99[^,],%19[^,],%lf", customer.name, customer.accountNumber, &customer.creditBalance);
        printf("Name: %s\n", customer.name);
        printf("Account Number: %s\n", customer.accountNumber);
        printf("Credit Balance: $%.2lf\n", customer.creditBalance);
        printf("-------------------------\n");
    }

    fclose(file);
}

// Function to update a customer's details
void updateCustomer() {
    char accountNumber[20];
    int index;
    FILE *file = fopen(FILENAME, "r+");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    printf("Enter the account number of the customer to update: ");
    getchar();  // Clear newline character left in buffer
    fgets(accountNumber, sizeof(accountNumber), stdin);
    accountNumber[strcspn(accountNumber, "\n")] = '\0';  // Remove newline character

    index = findCustomerIndex(accountNumber);
    if (index == -1) {
        printf("Customer with account number %s not found.\n", accountNumber);
        fclose(file);
        fclose(tempFile);
        return;
    }

    // Update the customer record
    Customer customer;
    printf("Enter new customer name: ");
    fgets(customer.name, sizeof(customer.name), stdin);
    customer.name[strcspn(customer.name, "\n")] = '\0';  // Remove newline character

    printf("Enter new credit balance: ");
    scanf("%lf", &customer.creditBalance);

    rewind(file);
    while (fgets(customer.name, sizeof(customer.name), file)) {
        if (strstr(customer.name, accountNumber) == NULL) {
            fputs(customer.name, tempFile);
        } else {
            fprintf(tempFile, "%s,%s,%.2lf\n", customer.name, accountNumber, customer.creditBalance);
        }
    }

    fclose(file);
    fclose(tempFile);
    remove(FILENAME);
    rename("temp.txt", FILENAME);
    printf("Customer details updated successfully!\n");
}

// Function to delete a customer record
void deleteCustomer() {
    char accountNumber[20];
    int index;
    FILE *file = fopen(FILENAME, "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    printf("Enter the account number of the customer to delete: ");
    getchar();  // Clear newline character left in buffer
    fgets(accountNumber, sizeof(accountNumber), stdin);
    accountNumber[strcspn(accountNumber, "\n")] = '\0';  // Remove newline character

    index = findCustomerIndex(accountNumber);
    if (index == -1) {
        printf("Customer with account number %s not found.\n", accountNumber);
        fclose(file);
        fclose(tempFile);
        return;
    }

    // Copy all records except the one to delete
    Customer customer;
    while (fgets(customer.name, sizeof(customer.name), file)) {
        if (strstr(customer.name, accountNumber) == NULL) {
            fputs(customer.name, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);
    remove(FILENAME);
    rename("temp.txt", FILENAME);
    printf("Customer deleted successfully!\n");
}

// Function to save customer to the file
void saveCustomer(const Customer *customer) {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%s,%s,%.2lf\n", customer->name, customer->accountNumber, customer->creditBalance);
    fclose(file);
}

// Function to find the index of a customer by account number
int findCustomerIndex(const char *accountNumber) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    int index = -1;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, accountNumber) != NULL) {
            index = 1;  // Just need to check if found or not
            break;
        }
    }

    fclose(file);
    return index;
}

// Function to load customers from file (initial load)
void loadCustomers() {
    FILE *file = fopen(FILENAME, "r");
    if (file != NULL) {
        fclose(file);  // Just to ensure file exists; data will be read when needed
    }
}
