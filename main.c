#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

typedef struct Ticket_node{
    struct Ticket_node* next;
    struct Ticket_node* prev;
    int id;
    char PNR[16];
    int age;
    char passenger_name[50];
    char seatpreference;
} Ticket_node;

typedef struct Ticket_list
{
    Ticket_node* Start;
    Ticket_node* Last;
} Ticket_list;

int search_PNR(Ticket_list* l, char PNR[]);

void info(Ticket_list* l, char name[], int age, char PNR[], char seatpref)
{
    Ticket_node *newrec = (Ticket_node*)malloc(sizeof(Ticket_node));

    newrec->next = NULL;
    newrec->prev = NULL;
    newrec->age = age;
    strcpy(newrec->passenger_name, name);
    strcpy(newrec->PNR, PNR);
    newrec->seatpreference = seatpref;

    if(l->Start == NULL)
    {
        l->Start = newrec;
        l->Last = newrec;
    }
    else
    {
        Ticket_node* p = l->Last;
        p->next = newrec;
        newrec->prev = p;
        l->Last = newrec;
    }

    printf("Ticket booked successfully. Generated PNR: %s\n", PNR);
}

void del(Ticket_list* l, char PNR[]) 
{
    Ticket_node* p = l->Start;

    while (p != NULL && strcmp(p->PNR, PNR) != 0)
        p = p->next;

    if (p == NULL)
    {
        printf("No such PNR found\n");
        return;
    }

    if (p->prev != NULL)
        p->prev->next = p->next;
    else
        l->Start = p->next;

    if (p->next != NULL)
        p->next->prev = p->prev;
    else
        l->Last = p->prev;

    free(p);
    printf("Ticket cancelled successfully.\n");
}

int search_PNR(Ticket_list* l, char PNR[])
{
    Ticket_node* p = l->Start;

    while(p != NULL)
    {
        if(strcmp(p->PNR, PNR) == 0)
            return 1;
        p = p->next;
    }
    return 0;
}

void display(Ticket_list* l)
{
    Ticket_node* p = l->Start;

    if(l->Start == NULL)
    {
        printf("No ticket records found\n");
        return;
    }

    printf("\n====================================================================\n");
    printf("| %-3s | %-20s | %-5s | %-12s | %-5s |\n", "No", "Passenger Name", "Age", "PNR", "Seat");
    printf("====================================================================\n");

    int count = 1;

    while(p != NULL)
    {
        printf("| %-3d | %-20s | %-5d | %-12s | %-5c |\n",
               count,
               p->passenger_name,
               p->age,
               p->PNR,
               p->seatpreference);
        p = p->next;
        count++;
    }

    printf("====================================================================\n");
}

void generate_random_pnr(char pnr[])
{
    const char letters[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for(int i = 0; i < 10; i++)
    {
        pnr[i] = letters[rand() % 36];
    }
    pnr[10] = '\0';
}

void generate_unique_pnr(Ticket_list* l, char pnr[])
{
    do
    {
        generate_random_pnr(pnr);
    } while (search_PNR(l, pnr));
}

int main()
{
    Ticket_list l;
    l.Start = NULL;
    l.Last = NULL;

    srand((unsigned int)time(NULL));

    int choice, age;
    char name[50], PNR[16], seatpref;

    while(1)
    {
        printf("\n========== MENU ==========\n");
        printf("1. Book Ticket\n");
        printf("2. Cancel Ticket\n");
        printf("3. Search by PNR\n");
        printf("4. Display All Tickets\n");
        printf("5. Exit\n");
        printf("==========================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                printf("Enter age: ");
                scanf("%d", &age);

                generate_unique_pnr(&l, PNR);

                printf("Enter seat preference (W/A/M): ");
                scanf(" %c", &seatpref);

                seatpref = toupper(seatpref);

                if(seatpref!='W' && seatpref!='A' && seatpref!='M')
                {
                    printf("Invalid seat preference\n");
                    break;
                }

                printf("Enter passenger name: ");
                scanf(" %[^\n]", name);

                info(&l, name, age, PNR, seatpref);
                break;

            case 2:
                printf("Enter PNR to cancel: ");
                scanf(" %15s", PNR);
                del(&l, PNR);
                break;

            case 3:
                printf("Enter PNR to search: ");
                scanf(" %15s", PNR);

                if(search_PNR(&l, PNR))
                    printf("PNR found\n");
                else
                    printf("PNR not found\n");
                break;

            case 4:
                display(&l);
                break;

            case 5:
                return 0;

            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}