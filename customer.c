#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void buy_item();
void rem_quantity(char inHSN[], int new_quantity);
void myflush(FILE *in);
void mypause(void);
void record_sale(char inHSN[], char desc[], int quantity);
void record_cust_sale(char inHSN[], char desc[], int quantity);
void record_cust_temp(char inHSN[], int quantity);
void gen_bill(char inHSN[], int new_quantity, int *prevbill);


int main()
{

    int choice;
    int prevBill=0;
    while (1)
    {
        printf("Select:\n1.Buy Items\n2.Exit\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            buy_item();
            break;

        case 2:
            exit(0);
            break;

        default:
            printf("\nInvalid input. Select from 1 to 2.");
            break;
        }
    }
    return 0;
}

void buy_item()
{
    FILE *fptr,*custFile,*custBill;
    fptr = fopen("inventory.txt", "r");
    if (fptr == NULL)
    {
        perror("Error opening file");
        return;
    }

    char inHSN[30];
    char HSN[30];
    char desc[30];
    char quantity[30];
    char mrp[30];
    char name[30];
    int new_quantity;
    int ch;
    int item_found = 0, prevbill = 0;
    printf("Enter your name: ");
    scanf("%s", name);
    custFile = fopen("customerBills.txt","a");
    fprintf(custFile, "-----------------------------------\n%s's Bill:\n", name);
    fclose(custFile);
    
    do
    {
    j:
        printf("\n\nEnter HSN code of the product: ");
        scanf(" %[^\n]%*c", inHSN);

        while (fscanf(fptr, "%s %s %s %s", HSN, desc, quantity, mrp) != EOF)
        {
            if (strcmp(inHSN, HSN) == 0)
            {
                item_found = 1;
                printf("\nItem found.\nProduct Description: %s \n", desc);
            j1:
                printf("\nEnter Quantity: ");
                scanf(" %d", &new_quantity);
                if (new_quantity > atoi(quantity))
                {
                    printf("Specified quantity exceeds the stock quantity. Current stock is: %s", quantity);
                    goto j1;
                }
            
                printf("\nDo you want to \n1.Continue shopping \n2.Change quantity \n3.generate bill? (1/2/3)\n->");
                scanf(" %d", &ch);

                if (ch == 1)
                {
                    printf("%d added to cart.", new_quantity);
            
                    record_cust_temp(inHSN, new_quantity);
                    goto j;
                }
                else if (ch == 2)
                {
                    goto j1;
                }

                else if(ch==3){
                    record_cust_temp(inHSN, new_quantity);
                   
                    gen_bill(inHSN, new_quantity, &prevbill);
                    break;
                }
            }
        }

        rewind(fptr);

        if (!item_found)
        {
            printf("No item with that HSN code found.\n");
        }
    } while (ch!=3);

    system("clear");
    printf("----------------Bill------------------\n\nName: %s\nTotal Bill: %d\n--------------------------------------\n\n\n\n", name, prevbill);
    custBill=fopen("customerBills.txt","a");
    fprintf(custBill, "Total Bill: %d\n\n",prevbill);
    fclose(custBill);
    myflush(stdin);
    mypause();
    system("clear");

    fclose(fptr);
}

void gen_bill(char inHSN[], int new_quantity, int *prevbill){
    
    int q;
    FILE *custFile,*fptr;
    custFile = fopen("customerTemp.txt", "r");
    fptr=fopen("inventory.txt","r");

    if (custFile == NULL)
    {
        perror("Error opening sales file");
        exit(0);
    }

    FILE *tempFile;
    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL)
    {
        perror("Error opening temp file");
        exit(0);
    }


    
    char nHSN[30];
    char nquantity[30];

    struct{
        char HSN[30];
        char desc[30];
        char quantity[30];
        char mrp[30];
    } data[100];

   int size=0;

    
    while (fscanf(fptr, "%s %s %s %s", data[size].HSN, data[size].desc, data[size].quantity, data[size].mrp) != EOF){
        size++;
    }

    int updateFlag[30] = {0};

    while (fscanf(custFile, "%s %s", nHSN, nquantity) != EOF)
{    

        for(int i=0; i<size; i++){
        if (strcmp(nHSN, data[i].HSN) == 0)
        {
            record_sale(data[i].HSN,data[i].desc,atoi(nquantity));
            q = atoi(data[i].quantity) - atoi(nquantity);
            record_cust_sale(data[i].HSN, data[i].desc, atoi(nquantity));

            fprintf(tempFile, "%s %s %d %s \n", data[i].HSN, data[i].desc, q, data[i].mrp);
            updateFlag[i]=1;

            *prevbill += atoi(data[i].mrp) * atoi(nquantity);
        }
        }
}

for (int i = 0; i < size; i++) {
    if (!updateFlag[i]) {
        fprintf(tempFile, "%s %s %s %s\n", data[i].HSN, data[i].desc, data[i].quantity, data[i].mrp);
    }

}


    fclose(fptr);
    fclose(tempFile);
    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");
    remove("customerTemp.txt");
    }



void record_sale(char inHSN[], char desc[], int quantity)
{
    FILE *reportF, *tempS;
    reportF = fopen("sales.txt", "r");
    tempS = fopen("tempSale.txt", "w");

    if (reportF == NULL || tempS == NULL)
    {
        perror("Error opening sales report file");
        exit(0);
    }

    char nHSN[30];
    char nDesc[30];
    char nquantity[30];

    struct
    {
    char HSN[30];
    char desc[30];
    int quantity;
    } data[100];

    int size = 0;
    int q = 0;

    while (fscanf(reportF, "%s %s %s", data[size].HSN, data[size].desc, nquantity) != EOF)
    {
        data[size].quantity = atoi(nquantity);
        size++;
    }

    int found = 0;

     for (int i = 0; i < size; i++)
    {
        if (strcmp(inHSN, data[i].HSN) == 0)
        {
            data[i].quantity += quantity;
            found = 1;
            break;
        }
    }

    if (!found)
    {
        strcpy(data[size].HSN, inHSN);
        strcpy(data[size].desc, desc);
        data[size].quantity = quantity;
        size++;
    }

    for (int i = 0; i < size; i++)
    {
        fprintf(tempS, "%s %s %d\n", data[i].HSN, data[i].desc, data[i].quantity);
    }

    fclose(reportF);
    fclose(tempS);
    remove("sales.txt");
    rename("tempSale.txt", "sales.txt");
}


void record_cust_sale(char inHSN[], char desc[], int quantity)
{
    FILE *custFile;
    custFile = fopen("customerBills.txt", "a");

    if (custFile == NULL)
    {
        perror("Error opening sales file");
        exit(0);
    }

    fprintf(custFile, "%s %s %d\n", inHSN, desc, quantity);

    fclose(custFile);
}

void record_cust_temp(char inHSN[], int quantity)
{
    FILE *custFile;
    custFile = fopen("customerTemp.txt", "a");

    if (custFile == NULL)
    {
        perror("Error opening sales file");
        exit(0);
    }

    fprintf(custFile, "%s %d\n", inHSN, quantity);

    fclose(custFile);
}


void myflush(FILE *in)
{
    int ch;

    do
        ch = fgetc(in);
    while (ch != EOF && ch != '\n');

    clearerr(in);
}

void mypause(void)
{
    printf("Press [Enter] to continue . . .");
    fflush(stdout);
    getchar();
}
