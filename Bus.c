#include <stdio.h>
#include <string.h>

struct Date
{
    int day;
    int month;
    int year;
};

struct Recharge
{
    int amount;
    struct Date rechargeDate;
};

struct BusPass
{
    int passID;
    int prn;
    char studentName[50];
    char route[80];
    int routeCode;
    int rechargeAmount;
    int isActivated;             
    struct Date lastRechargeDate;
    struct Recharge rechargeHistory[10];
    int rechargeCount;
};



#define TOTAL_ROUTES 10

char routeNames[TOTAL_ROUTES][60] =
{
    "Ishwarpur to Satara",
    "Ishwarpur to Sangli",
    "Ishwarpur to Vita",
    "Ishwarpur to Kirloskarwadi",
    "Ishwarpur to Atpadi",
    "Ishwarpur to Tasgaon",
    "Ishwarpur to Palus",
    "Ishwarpur to Shirala",
    "Ishwarpur to Walwa",
    "Ishwarpur to Miraj"
};

char routeCodes[TOTAL_ROUTES][10] =
{
    "ISP-01", "ISP-02", "ISP-03", "ISP-04", "ISP-05",
    "ISP-06", "ISP-07", "ISP-08", "ISP-09", "ISP-10"
};

int routeDistances[TOTAL_ROUTES] =
{
    55, 18, 22, 12, 35, 28, 20, 30, 8, 25
};

int routeAmounts[TOTAL_ROUTES] =
{
    900, 450, 500, 300, 700, 600, 450, 650, 250, 550
};



struct BusPass pass[1000];
int count      = 0;
int nextPassID = 1001;



void clearBuffer()
{
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}



int dateToDays(int d, int m, int y)
{
    return (y * 365) + (y / 4) - (y / 100) + (y / 400)
           + ((m * 306 + 5) / 10) + d;
}



void showRoutes()
{
    int i;

    printf("\n-------------------------------------------------------------\n");
    printf("  No. | Code   | Route                          | Dist  | Amount\n");
    printf("-------------------------------------------------------------\n");

    for(i = 0; i < TOTAL_ROUTES; i++)
    {
        printf("  %2d  | %-6s | %-30s | %3d km | Rs.%d\n",
               i + 1,
               routeCodes[i],
               routeNames[i],
               routeDistances[i],
               routeAmounts[i]);
    }

    printf("-------------------------------------------------------------\n");
    printf("  Amount shown is for 30-day pass validity\n");
    printf("-------------------------------------------------------------\n");
}

/* ================= REGISTER PASS ================= */
/* Only saves student details. Pass is NOT activated  */
/* until the student recharges separately.            */

void registerPass()
{
    int choice;

    if(count >= 1000)
    {
        printf("\nStorage Full!\n");
        return;
    }

    printf("\n===== REGISTER FOR BUS PASS =====\n");

    printf("Enter Student Name : ");
    scanf(" %[^\n]", pass[count].studentName);

    printf("Enter PRN Number   : ");
    scanf("%d", &pass[count].prn);
    clearBuffer();

    printf("\nAvailable Routes (Ishwarpur Depot):\n");
    showRoutes();

    printf("\nEnter Route Number (1 to %d) : ", TOTAL_ROUTES);
    scanf("%d", &choice);
    clearBuffer();

    if(choice < 1 || choice > TOTAL_ROUTES)
    {
        printf("\nInvalid Route Number! Registration cancelled.\n");
        return;
    }

    /* Fill route details */
    pass[count].routeCode      = choice;
    pass[count].rechargeAmount = routeAmounts[choice - 1];
    pass[count].isActivated    = 0;    /* NOT activated yet */
    pass[count].rechargeCount  = 0;

    sprintf(pass[count].route, "%s : %s",
            routeCodes[choice - 1],
            routeNames[choice - 1]);

    /* Auto-generate Pass ID */
    pass[count].passID = nextPassID;
    nextPassID++;

    printf("\n----- Registration Successful! -----\n");
    printf("Pass ID         : BP%d\n",             pass[count].passID);
    printf("Student Name    : %s\n",               pass[count].studentName);
    printf("PRN Number      : %d\n",               pass[count].prn);
    printf("Route           : %s\n",               pass[count].route);
    printf("Recharge Amount : Rs.%d / 30 days\n",  pass[count].rechargeAmount);
    printf("Pass Status     : NOT ACTIVATED\n");
    printf("Note            : Please recharge to activate your pass.\n");
    printf("------------------------------------\n");

    count++;
}

/* ================= VERIFY PASS ================= */

void verifyPass()
{
    int id, i, found = 0;
    int td, tm, ty, difference;

    printf("\nEnter Pass ID : ");
    scanf("%d", &id);
    clearBuffer();

    printf("Enter Today's Date (dd mm yyyy) : ");
    scanf("%d %d %d", &td, &tm, &ty);
    clearBuffer();

    for(i = 0; i < count; i++)
    {
        if(pass[i].passID == id)
        {
            found = 1;

            printf("\n===== PASS DETAILS =====\n");
            printf("Pass ID         : BP%d\n",  pass[i].passID);
            printf("Student Name    : %s\n",    pass[i].studentName);
            printf("PRN Number      : %d\n",    pass[i].prn);
            printf("Route           : %s\n",    pass[i].route);
            printf("Recharge Amount : Rs.%d\n", pass[i].rechargeAmount);

            /* Check if pass was ever recharged/activated */
            if(pass[i].isActivated == 0)
            {
                printf("Pass Status     : *** NOT ACTIVATED ***\n");
                printf("Action Required : Please recharge Rs.%d to activate.\n",
                       pass[i].rechargeAmount);
            }
            else
            {
                printf("Last Recharge   : %02d-%02d-%04d\n",
                       pass[i].lastRechargeDate.day,
                       pass[i].lastRechargeDate.month,
                       pass[i].lastRechargeDate.year);

                difference =
                    dateToDays(td, tm, ty) -
                    dateToDays(pass[i].lastRechargeDate.day,
                               pass[i].lastRechargeDate.month,
                               pass[i].lastRechargeDate.year);

                printf("Days Used       : %d\n", difference);

                if(difference >= 0 && difference <= 30)
                {
                    printf("Pass Status     : *** VALID ***\n");
                    printf("Remaining Days  : %d\n", 30 - difference);
                }
                else
                {
                    printf("Pass Status     : *** EXPIRED ***\n");
                    printf("Please Recharge : Rs.%d\n", pass[i].rechargeAmount);
                }
            }

            break;
        }
    }

    if(found == 0)
        printf("\nPass Not Found!\n");
}

/* ================= RECHARGE PASS ================= */

void rechargePass()
{
    int id, i, d, m, y;
    char confirm;

    printf("\nEnter Pass ID : ");
    scanf("%d", &id);
    clearBuffer();

    for(i = 0; i < count; i++)
    {
        if(pass[i].passID == id)
        {
            if(pass[i].rechargeCount >= 10)
            {
                printf("\nRecharge History Full!\n");
                return;
            }

            printf("\nPass Found  : %s\n",  pass[i].studentName);
            printf("Route       : %s\n",   pass[i].route);
            printf("Amount Due  : Rs.%d\n", pass[i].rechargeAmount);

            if(pass[i].isActivated == 0)
                printf("Note        : This will ACTIVATE your pass.\n");

            printf("Confirm Recharge? (y/n) : ");
            scanf(" %c", &confirm);
            clearBuffer();

            if(confirm != 'y' && confirm != 'Y')
            {
                printf("Recharge Cancelled.\n");
                return;
            }

            printf("Enter Recharge Date (dd mm yyyy) : ");
            scanf("%d %d %d", &d, &m, &y);
            clearBuffer();

            /* Save to history */
            pass[i].rechargeHistory[pass[i].rechargeCount].amount             = pass[i].rechargeAmount;
            pass[i].rechargeHistory[pass[i].rechargeCount].rechargeDate.day   = d;
            pass[i].rechargeHistory[pass[i].rechargeCount].rechargeDate.month = m;
            pass[i].rechargeHistory[pass[i].rechargeCount].rechargeDate.year  = y;

            /* Update last recharge date */
            pass[i].lastRechargeDate.day   = d;
            pass[i].lastRechargeDate.month = m;
            pass[i].lastRechargeDate.year  = y;

            pass[i].rechargeCount++;
            pass[i].isActivated = 1;    /* Mark as activated */

            printf("\nRecharge Successful!\n");
            printf("Amount Paid  : Rs.%d\n", pass[i].rechargeAmount);
            printf("Pass Status  : ACTIVATED\n");
            printf("Valid For    : Next 30 Days\n");
            return;
        }
    }

    printf("\nPass Not Found!\n");
}

/* ================= VIEW RECHARGE HISTORY ================= */

void viewRechargeHistory()
{
    int id, i, j;

    printf("\nEnter Pass ID : ");
    scanf("%d", &id);
    clearBuffer();

    for(i = 0; i < count; i++)
    {
        if(pass[i].passID == id)
        {
            printf("\nPass : BP%d | %s | %s\n",
                   pass[i].passID,
                   pass[i].studentName,
                   pass[i].route);

            if(pass[i].rechargeCount == 0)
            {
                printf("No Recharge History Found!\n");
                printf("Pass is NOT ACTIVATED yet.\n");
                return;
            }

            printf("\n===== RECHARGE HISTORY =====\n");

            for(j = 0; j < pass[i].rechargeCount; j++)
            {
                printf("\nRecharge %d\n", j + 1);
                printf("Amount : Rs.%d\n",
                       pass[i].rechargeHistory[j].amount);
                printf("Date   : %02d-%02d-%04d\n",
                       pass[i].rechargeHistory[j].rechargeDate.day,
                       pass[i].rechargeHistory[j].rechargeDate.month,
                       pass[i].rechargeHistory[j].rechargeDate.year);
            }

            return;
        }
    }

    printf("\nPass Not Found!\n");
}

/* ================= MAIN ================= */

int main()
{
    int choice;

    while(1)
    {
        printf("\n====================================\n");
        printf("   DIGITAL BUS PASS SYSTEM\n");
        printf("   Depot : Ishwarpur\n");
        printf("====================================\n");
        printf("1. Register For Bus Pass\n");
        printf("2. Verify Bus Pass\n");
        printf("3. Recharge Pass\n");
        printf("4. View Recharge History\n");
        printf("5. Show All Routes\n");
        printf("6. Exit\n");
        printf("====================================\n");
        printf("Enter Your Choice : ");

        if(scanf("%d", &choice) != 1)
        {
            clearBuffer();
            printf("\nInvalid Input! Please enter a number.\n");
            continue;
        }
        clearBuffer();

        switch(choice)
        {
            case 1: registerPass();        break;
            case 2: verifyPass();          break;
            case 3: rechargePass();        break;
            case 4: viewRechargeHistory(); break;
            case 5: showRoutes();          break;
            case 6:
                printf("\nThank You! Goodbye!\n");
                return 0;
            default:
                printf("\nInvalid Choice! Enter 1 to 6.\n");
        }
    }
}
