#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                &r->id,
		        &r->userId,
		        name,
                &r->accountNbr,
                &r->deposit.month,
                &r->deposit.day,
                &r->deposit.year,
                r->country,
                &r->phone,
                &r->amount,
                r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    char line[256]; 
    int len = 0;

    rewind(ptr);

    while (fgets(line, sizeof(line), ptr)) {
        if (line[strlen(line) - 1] == '\n') { 
            len++; 
        }
    }
    r.id = (len / 2); 

    fseek(ptr, 0, SEEK_END);

    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
            u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    char input[10];
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    fgets(input, sizeof(input), stdin);
    if (strchr(input, ' ') != NULL)
    {
        printf("\n✖ Input contains space. Please try again.\n");
        goto invalid;
    }
    sscanf(input, "%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");
    FILE *uf = fopen("./data/users.txt", "r");
    char line[100];
    char fileUserName[50];
    char password[50];
    int fileUserId;
    char input[100];

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    // Find the user ID from users.txt
    while (fgets(line, sizeof(line), uf))
    {
        sscanf(line, "%d %s %s", &fileUserId, fileUserName, password);
        if (strcmp(fileUserName, u.name) == 0)
        {
            u.id = fileUserId;
            break;
        }
    }
    rewind(uf);

    // Validate Date Input
    while (1)
    {
        printf("\nEnter today's date (mm/dd/yyyy): ");
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year) == 3)
        {
            if (r.deposit.month >= 1 && r.deposit.month <= 12 &&
                r.deposit.day >= 1 && r.deposit.day <= 31 &&
                r.deposit.year > 0)
            {
                break; // Valid date
            }
            else
            {
                printf("✖ Invalid date values. Please enter a valid date.\n");
            }
        }
        else
        {
            printf("✖ Invalid format. Use mm/dd/yyyy.\n");
        }
    }

    // Validate Account Number
    while (1)
    {
        printf("\nEnter the account number: ");
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &r.accountNbr) == 1)
        {
            break;
        }
        else
        {
            printf("\n✖ Invalid account number. Please enter an integer.\n");
        }
    }

    // Check if account already exists
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }

    // Validate Country Input
    while (1)
    {
        int valid = 1;
        printf("\nEnter the country: ");
        fgets(r.country, sizeof(r.country), stdin);
        r.country[strcspn(r.country, "\n")] = 0; // Remove newline character
        for (int i = 0; r.country[i] != '\0'; i++)
        {
            if (!isalpha(r.country[i]))
            {
                valid = 0;
                break;
            }
        }
        if (valid)
            break;
        else
            printf("✖ Invalid country name. Please enter alphabet characters only.\n");
    }

    // Validate Phone Number
    while (1)
    {
        printf("\nEnter the phone number (9 digits): ");
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &r.phone) == 1 && r.phone >= 100000000 && r.phone <= 999999999)
        {
            break;
        }
        else
        {
            printf("✖ Invalid phone number. Please enter exactly 9 digits.\n");
        }
    }

    // Validate Deposit Amount
    while (1)
    {
        printf("\nEnter amount to deposit (max $1,000,000): $");
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%lf", &r.amount) == 1)
        {
            if (r.amount >= 0 && r.amount <= 1000000) break;
            else printf("✖ Amount must be between $0 and $1,000,000.\n");
        }
        else
        {
            printf("✖ Invalid amount. Please enter a valid number.\n");
        }
    }

    // Validate Account Type
    while (1)
    {
        printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01 (1 year)\n\t-> fixed02 (2 years)\n\t-> fixed03 (3 years)\n\n\tEnter your choice: ");
        fgets(r.accountType, sizeof(r.accountType), stdin);
        r.accountType[strcspn(r.accountType, "\n")] = 0; // Remove newline character
        if (strcmp(r.accountType, "saving") == 0 || strcmp(r.accountType, "current") == 0 ||
            strcmp(r.accountType, "fixed01") == 0 || strcmp(r.accountType, "fixed02") == 0 ||
            strcmp(r.accountType, "fixed03") == 0)
        {
            break;
        }
        else
        {
            printf("✖ Invalid account type. Please choose from the available options.\n");
        }
    }

    // Save Validated Data
    saveAccountToFile(pf, u, r);

    fclose(pf);
    fclose(uf);
    success(u);
}

void updateAccount(struct User u)
{
    struct Record cr;
    char userName[50];
    int accountNumber, found = 0;
    int updateChoice;
    int originalUserId; // Store the original userId to preserve it
    char input[100]; // Buffer for input

    FILE *pf = fopen(RECORDS, "r"); // Open existing file for reading
    FILE *temp = fopen("temp.txt", "w"); // Temporary file for writing
    if (pf == NULL || temp == NULL)
    {
        perror("Error opening file");
        return;
    }
    system("clear");
    printf("\t\t===== Update Account Details =====\n");
    printf("\nEnter the account number to update: ");
    fgets(input, sizeof(input), stdin);
    if (strchr(input, ' ') != NULL)
    {
        printf("\n✖ Input contains space. Please try again.\n");
        fclose(pf);
        fclose(temp);
        remove("temp.txt");
        return;
    }
    sscanf(input, "%d", &accountNumber);
    
    while (getAccountFromFile(pf, userName, &cr))
    {
        // Store the original userId from the file
        originalUserId = cr.userId;
        
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == accountNumber)
        {
            found = 1;
            
            printf("\nAccount found! Choose what to update:\n\n");
            printf("1. Update country\n");
            printf("2. Update phone number\n\n");
            printf("Enter your choice (1 or 2): ");
            fgets(input, sizeof(input), stdin);
            if (strchr(input, ' ') != NULL)
            {
                printf("\n✖ Input contains space. Please try again.\n");
                fclose(pf);
                fclose(temp);
                remove("temp.txt");
                return;
            }
            sscanf(input, "%d", &updateChoice);
            
            if (updateChoice == 1)
            {
                while (1)
                {
                    int valid = 1;
                    printf("\nEnter updated country: ");
                    fgets(cr.country, sizeof(cr.country), stdin);
                    if (strchr(cr.country, ' ') != NULL)
                    {
                        printf("\n✖ Input contains space. Please try again.\n");
                        fclose(pf);
                        fclose(temp);
                        remove("temp.txt");
                        return;
                    }
                    cr.country[strcspn(cr.country, "\n")] = 0; // Remove newline character
                    for (int i = 0; cr.country[i] != '\0'; i++)
                    {
                        if (!isalpha(cr.country[i]))
                        {
                            valid = 0;
                            break;
                        }
                    }
                    if (valid)
                    {
                        printf("\n✔ Country updated successfully!\n");
                        goto done;
                        break;
                    }
                    else
                    {
                        printf("\n✖ Invalid country name. Please enter alphabet characters only.\n");
                    }
                }
            }
            else if (updateChoice == 2)
            {
                while (1)
                {
                    printf("\nEnter updated phone number (9 digits): ");
                    fgets(input, sizeof(input), stdin);
                    if (strchr(input, ' ') != NULL)
                    {
                        printf("\n✖ Input contains space. Please try again.\n");
                        fclose(pf);
                        fclose(temp);
                        remove("temp.txt");
                        return;
                    }
                    if (sscanf(input, "%d", &cr.phone) == 1 && cr.phone >= 100000000 && cr.phone <= 999999999)
                    {
                        printf("\n✔ Phone number updated successfully!\n");
                        goto done;
                        break;
                    }
                    else
                    {
                        printf("\n✖ Invalid phone number. Please enter exactly 9 digits.\n");
                    }
                }
            }
            else
            {
                printf("\n✖ Invalid choice. No updates made.\n");
            }
            done:
            fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                cr.id, originalUserId, userName, cr.accountNbr,
                cr.deposit.month, cr.deposit.day, cr.deposit.year,
                cr.country, cr.phone, cr.amount, cr.accountType);
        }
        else
        {
            // Keep existing records unchanged
            fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                cr.id, originalUserId, userName, cr.accountNbr,
                cr.deposit.month, cr.deposit.day, cr.deposit.year,
                cr.country, cr.phone, cr.amount, cr.accountType);
        }
    }
    
    fclose(pf);
    fclose(temp);
    
    if (!found)
    {
        printf("\n✖ Account does not exist!\n");
        remove("temp.txt");
    }
    else
    {
        remove(RECORDS);
        rename("temp.txt", RECORDS);
        printf("\nAccount information has been updated in the record.\n");
    }

    success(u);
}

void checkSpecificAccount(struct User u)
{
    struct Record r;
    char userName[50];
    int accountNumber, found = 0;
    char input[100]; // Buffer for input

    FILE *pf = fopen(RECORDS, "r"); // Open file for reading
    if (pf == NULL)
    {
        perror("Error opening file");
        return;
    }

    system("clear");
    printf("\t\t===== Check Specific Account =====\n\n");

    