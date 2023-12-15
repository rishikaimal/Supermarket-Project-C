#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int new_item();
int update();
int add_item();
int display_item();

int main()
{
    int choice;

    while (1)
    {
        printf("\n\nSelect operation: \n1.New Item \n2.Update item \n3.Add item\n4.Display Inventory\n5.Exit\n-> ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            new_item();
            break;

        case 2:
            update();
            break;

        case 3:
            add_item();
            break;
        
        case 4:
            display_item();
            break;

        case 5:
            exit(0);
            break;

        default:
            printf("\nInvalid input. Select from 1 to 5.");
            break;
        }
    }
}

int new_item()
{
    int quan;
    int mrp;
    char desc[20], hsn[10];

    FILE *fptr;
    fptr = fopen("/home/zen/Desktop/codes/inventory.txt", "a");

    printf("Enter HSN Code:");
    scanf(" %[^\n]%*c", hsn);
    printf("Enter Product Description:");
    scanf(" %[^\n]%*c", desc);
    printf("Enter Quantity:");
    scanf("%d", &quan);
    printf("Enter MRP:");
    scanf("%d", &mrp);

    fflush(stdin);
    fprintf(fptr, "%s %s %d %d\n", hsn, desc, quan, mrp);
    fflush(fptr);
    fclose(fptr);
    return 0;
}

int update(){

    FILE *fptr;
    fptr = fopen("/home/zen/Desktop/codes/inventory.txt", "r");

    if (fptr == NULL)
    {
        perror("Error opening file");
        exit(0);
    }

    FILE *temp;
    temp = fopen("temp.txt", "w");
    if (temp == NULL)
    {
        perror("Error opening temp file");
        exit(0);
    }

    char inHSN[30];
    char HSN[30];
    char desc[30];
    char quantity[30];
    char mrp[30];
    int new_quantity;
    int new_mrp;

    printf("Enter HSN code of the item to update: ");
    scanf(" %[^\n]%*c", inHSN);

    while (fscanf(fptr, "%s %s %s %s", HSN, desc, quantity, mrp) != EOF)
    {
        if (strcmp(inHSN, HSN) == 0)
        {
            printf("\nItem found with HSN_CODE: %s \n", inHSN);
            printf("Enter New Quantity: ");
            scanf(" %d", &new_quantity);
            printf("Enter New MRP: ");
            scanf(" %d", &new_mrp);

            fprintf(temp, "%s %s %d %d \n", HSN, desc, new_quantity, new_mrp);
        }
        else
        {
            fprintf(temp, "%s %s %s %s \n", HSN, desc, quantity, mrp);
        }

    }

    printf("\n%s has been updated.\nNew quantity: %d and New mrp: %d", inHSN, new_quantity, new_mrp);
    fclose(fptr);
    fclose(temp);

    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");

    return 0;
}

int add_item()
{
    FILE *fptr;
    fptr = fopen("/home/zen/Desktop/codes/inventory.txt", "r");

    if (fptr == NULL)
    {
        perror("Error opening file");
        exit(0);
    }

    FILE *temp = fopen("temp.txt", "w");

    if (temp == NULL)
    {
        perror("Error opening temp file");
        exit(0);
    }

    char inHSN[30];
    char HSN[30];
    char desc[30];
    char quantity[30];
    char mrp[30];
    int new_quantity;

    printf("Enter HSN code of the item to add: ");
    scanf(" %[^\n]%*c", inHSN);

    while (fscanf(fptr, "%s %s %s %s", HSN, desc, quantity, mrp) != EOF)
    {
        if (strcmp(inHSN, HSN) == 0)
        {
            printf("\nItem found with HSN_CODE : %s \n", inHSN);
            printf("Enter New Quantity: ");
            scanf(" %d", &new_quantity);
            fprintf(temp, "%s %s %d %s \n", HSN, desc, new_quantity, mrp);
    
        }
        else
        {
            fprintf(temp, "%s %s %s %s \n", HSN, desc, quantity, mrp);
        }
    }

    fclose(fptr);
    fclose(temp);

    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");
    printf("\nQuantity of %s has been increased to %d.", inHSN, new_quantity);
    return 0;
}


int display_item(){
    
    FILE *fptr;
    char ch;
    fptr = fopen("/home/zen/Desktop/codes/inventory.txt", "r");
    printf("Warehouse Inventory:\n\n\n");
    do {
        ch = fgetc(fptr);
        printf("%c", ch);
    } while (ch != EOF);
    fflush(fptr);
    fclose(fptr);
    return 0;
}
