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
