#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account {
    int acc_no;
    char name[50];
    float balance;
};

void addAccount() {
    struct Account acc;
    FILE *fp = fopen("accounts.txt", "a");
    if (!fp) { printf("Error opening file.\n"); return; }

    printf("Enter Account Number: ");
    scanf("%d", &acc.acc_no);
    printf("Enter Name: ");
    scanf(" %[^\n]%*c", acc.name);
    printf("Enter Initial Balance: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(struct Account), 1, fp);
    fclose(fp);
    printf("Account created successfully!\n");
}

void displayAccounts() {
    struct Account acc;
    FILE *fp = fopen("accounts.txt", "r");
    if (!fp) { printf("No accounts found.\n"); return; }

    printf("\n--- Account List ---\n");
    while (fread(&acc, sizeof(struct Account), 1, fp)) {
        printf("Acc No: %d, Name: %s, Balance: %.2f\n", acc.acc_no, acc.name, acc.balance);
    }
    fclose(fp);
}

void depositWithdraw(int type) {
    int acc_no;
    float amount;
    struct Account acc;
    FILE *fp = fopen("accounts.txt", "r+");
    if (!fp) { printf("No accounts found.\n"); return; }

    printf("Enter Account Number: ");
    scanf("%d", &acc_no);

    int found = 0;
    while (fread(&acc, sizeof(struct Account), 1, fp)) {
        if (acc.acc_no == acc_no) {
            found = 1;
            if (type == 1) { // Deposit
                printf("Enter amount to deposit: ");
                scanf("%f", &amount);
                acc.balance += amount;
            } else { // Withdraw
                printf("Enter amount to withdraw: ");
                scanf("%f", &amount);
                if (amount > acc.balance) {
                    printf("Insufficient balance!\n");
                    fclose(fp);
                    return;
                }
                acc.balance -= amount;
            }
            fseek(fp, -sizeof(struct Account), SEEK_CUR);
            fwrite(&acc, sizeof(struct Account), 1, fp);
            printf("Transaction successful! New Balance: %.2f\n", acc.balance);
            break;
        }
    }

    if (!found) printf("Account not found.\n");
    fclose(fp);
}

int main() {
    int choice;
    do {
        printf("\n1. Add Account\n2. Display Accounts\n3. Deposit\n4. Withdraw\n5. Exit\nChoose: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1: addAccount(); break;
            case 2: displayAccounts(); break;
            case 3: depositWithdraw(1); break;
            case 4: depositWithdraw(2); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while(choice != 5);

    return 0;
}
