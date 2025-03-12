#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

struct Patients {
    int PIN;
    char NAME[40];
    int AGE;
    char ADDRESS[100];
};

struct Appointment {
    int PIN;
    char NAME[40];
    char DOCTOR[100];
    char DATE[10];
};

void readPIN(int);
void openAcc(struct Patients *p);
void appointmentDoctor(int, char[100]);
int chooseDoctor(int);
void checkAppointment(int pin);

void readPIN(int pin) {
    FILE *fp = fopen("data.dat", "rb");
    if (fp == NULL) {
        perror("Failed to open file");
        return;
    }

    struct Patients p;
    int found = 0;

    while (fread(&p, sizeof(struct Patients), 1, fp) == 1) {
        if (p.PIN == pin) {
            printf("Patient record found:\t");
            printf("PIN: %d\t", p.PIN);
            printf("Name: %s\t", p.NAME);
            printf("Age: %d\t", p.AGE);
            printf("Address: %s\n", p.ADDRESS);
            found = 1;
            chooseDoctor(pin);
            break;
        }
    }

    if (!found) {
        printf("Patient with PIN %d not found.\n", pin);
    }

    fclose(fp);
}

void openAcc(struct Patients *p) {
    // Read from Keyboard
    printf("Enter Patient Identity Number (PIN): ");
    scanf("%d", &p->PIN);
    getchar();  

    printf("Enter name: ");
    fgets(p->NAME, sizeof(p->NAME), stdin);
    p->NAME[strcspn(p->NAME, "\n")] = '\0';

    printf("Enter age: ");
    scanf("%d", &p->AGE);
    getchar();  

    printf("Enter address: ");
    fgets(p->ADDRESS, sizeof(p->ADDRESS), stdin);
    p->ADDRESS[strcspn(p->ADDRESS, "\n")] = '\0';

    FILE *fp = fopen("data.dat", "ab");
    if (fp == NULL) {
        perror("Failed to open file");
        return;
    }
    fwrite(p, sizeof(struct Patients), 1, fp);
    fclose(fp);
    chooseDoctor(p->PIN);
}

void appointmentDoctor(int pin, char doctor[100]) {
    char date[10], choice,option;
    struct Appointment a;
    struct Patients p;

    FILE *fp1 = fopen("data.dat", "rb");
    if (fp1 == NULL) {
        perror("Failed to open file");
        return;
    }

    do {
        
        int num = (rand() % 10) + 1;
        printf("BOOKING APPOINTMENT FOR DOCTOR %s\n",doctor);
        printf("Enter the date you want to book an appointment on? (DD/MM/YY)\n");
        fgets(date, sizeof(date), stdin);
        date[strcspn(date, "\n")] = '\0';
        
        if (num < 8) {
            printf("The %s is not available.",doctor);
            
            
            
        } else {
            printf("The %s is available. Do you want to confirm your appointment? (y/n) ",doctor);
            scanf(" %c", &option);
            getchar();  
            if (option == 'y' || option == 'Y') {
                a.PIN = pin;

                while (fread(&p, sizeof(struct Patients), 1, fp1) == 1) {
                    if (p.PIN == pin) {
                        strcpy(a.NAME, p.NAME);
                        break;
                    }
                }
                strcpy(a.DOCTOR, doctor);
                strcpy(a.DATE, date);
            
                FILE *fp2 = fopen("appointment.dat", "ab");
                if (fp2 == NULL) {
                    perror("Failed to open file");
                    exit(1);
                }
                fwrite(&a, sizeof(struct Appointment), 1, fp2);
                fclose(fp2);
                printf("Your appointment has been booked for %s.\n",doctor);
            }
        }
        printf("Do you want to book an appointment on another date? (y/n) ");
        scanf(" %c", &choice);
        getchar();  

    } while (choice == 'y' || choice == 'Y');
    fclose(fp1);
}

int chooseDoctor(int pin) {
    srand(time(0));
    int operation;
    char choice;
    do{ 
    printf("\n-----Which Doctor do you want to consult?-----\n");
    printf("1. Dr.Annu Roy, MBBS, MD(Ped), DM (Ped Critical care)\n");
    printf("2. Dr.Zacharia, MBBS, PGDFM\n");
    printf("3. Dr.Unnikrishnan P, MBBS, MD General Medicine\n");
    printf("Choose the Doctor: ");
    scanf(" %d", &operation);
    getchar();

    switch (operation) {
        case 1:
            appointmentDoctor(pin, "DR.ANNU ROY");
            break;
        case 2:
            appointmentDoctor(pin, "DR.ZACHARIA");
            break;
        case 3:
            appointmentDoctor(pin, "DR.UNNIKRISHNAN");
            break;
        default:
            printf("Error! Invalid operation\n");
            break;
    printf("Do you want to book another appointment? (y/n) ");
    scanf(" %c", &choice);
    getchar();  
    }
    }while (choice == 'y' || choice == 'Y');
    return 0;
}

void checkAppointment(int pin) {
    printf("Checking your Appointment.\n");
    struct Appointment a;
    int found = 0;
    FILE *fp = fopen("appointment.dat", "rb");
    if (fp == NULL) {
        perror("Failed to open file");
        return;
    }
    
    

    while (fread(&a, sizeof(struct Appointment), 1, fp) == 1) {
        if (a.PIN == pin) {
            printf("Appointment record found:\n");
            printf("Name: %s\t", a.NAME);
            printf("Doctor: %s\t", a.DOCTOR);
            printf("Date: %s\n", a.DATE);
            found = 1;
            break;;
        }
    }

    if (!found) {
        printf("Appointment with PIN %d not found.\n", pin);
    }

    fclose(fp);
}

int main() {
    int operation, PIN;
    char choice;
    struct Patients patients;

    do {
        printf("\n-----Welcome to St.Sebastian Hospital-----\n");
        printf("--------How may I help you---------\n");
        printf("1. Do you want to book an appointment.\n");
        printf("2. Check my Appointment.\n");
        printf("3. Do you want to have a follow up.\n");
        printf("4. EXIT\n");
      
        printf("Enter an operation: ");
        scanf(" %d", &operation);

        switch (operation) {
            case 1:
                printf("Do you have a file in this hospital already? (y/n)\n");
                scanf(" %c", &choice);
                getchar();  
                if (choice == 'y' || choice == 'Y') {
                    printf("Enter the PIN: ");
                    scanf("%d", &PIN);
                    getchar();  
                    readPIN(PIN);
                } else if (choice == 'n' || choice == 'N') {
                    openAcc(&patients);
                }
                break;
            case 2:

                printf("Enter the PIN: ");
                scanf("%d", &PIN);
                getchar();  
                checkAppointment(PIN);
                break;
            case 3:
                printf("Continue your FOllow Up.\n");
                printf("Enter the PIN: ");
                scanf("%d", &PIN);
                getchar();
                readPIN(PIN);
                break;
            case 4:
                printf("EXITING....\n");
                break;
            default:
                printf("Error! Invalid operation\n");
                break;
        }

        printf("Do you want to continue? (y/n): ");
        scanf(" %c", &choice);
        getchar(); 

    } while (choice == 'y' || choice == 'Y');

    return 0;
}
