#include "include/input_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME 50
#define MAX_PHONE 20
#define MAX_EMAIL 50
#define MAX_ID 50  
#define ROWS 5
#define COLS 6
#define TOTAL_SEATS (ROWS * COLS)

// STRUCT TO STORE PASSENGER DETAILS
typedef struct Passenger {
    char id[MAX_ID];     
    char name[MAX_NAME]; 
    char email[MAX_EMAIL];
    char phone[MAX_PHONE]; 
    char seatNumber[10];
    struct Passenger *next;
} Passenger;

Passenger *head = NULL;
int seatMap[ROWS][COLS]; // 0 = available, 1 = booked

// ===== Function Prototypes =====
void displaySeats();
void bookSeat();
void addPassenger(char id[], char name[], char email[], char phone[], char seatNumber[]);
Passenger* searchPassengerByName(char name[]);
Passenger* searchPassengerBySeat(char seatNumber[]);
int deletePassenger(char seatNumber[]);
void displayAllPassengers();
void searchForPassenger();
int getSearchChoice();
void cancelBooking();
void saveDataToFile();
void loadDataFromFile();
void viewAirlineRules(); // NEW FUNCTION

// ===== File Handling =====
void saveDataToFile() {
    FILE *fp = fopen("booking_data.txt", "w");
    if (!fp) {
        printf("Error saving data!\n");
        return;
    }

    fprintf(fp, "%d %d\n", ROWS, COLS);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fprintf(fp, "%d ", seatMap[i][j]);
        }
        fprintf(fp, "\n");
    }

    Passenger *temp = head;
    while (temp != NULL) {
        fprintf(fp, "%s,%s,%s,%s,%s\n",
                temp->id,
                temp->name,
                temp->email,
                temp->phone,
                temp->seatNumber);
        temp = temp->next;
    }

    fclose(fp);
    printf("Data saved successfully!\n");
}

void loadDataFromFile() {
    FILE *fp = fopen("booking_data.txt", "r");
    if (!fp) {
        printf("No saved data found. Starting fresh.\n");
        memset(seatMap, 0, sizeof(seatMap));
        return;
    }

    int rows, cols;
    fscanf(fp, "%d %d", &rows, &cols);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fscanf(fp, "%d", &seatMap[i][j]);
        }
    }

    char line[200];
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)) {
        Passenger *newP = (Passenger *)malloc(sizeof(Passenger));
        sscanf(line, "%49[^,],%49[^,],%49[^,],%19[^,],%9s",
               newP->id,
               newP->name,
               newP->email,
               newP->phone,
               newP->seatNumber);
        newP->next = head;
        head = newP;
    }

    fclose(fp);
    printf("Data loaded successfully!\n");
}

// ===== NEW FUNCTION =====
void viewAirlineRules() {
    FILE *file = fopen("rules.txt", "r");
    if (!file) {
        printf("Error: rules.txt not found. Please create the file in the program directory.\n");
        return;
    }

    char line[256];
    printf("\n--- Airline Rules & Regulations ---\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    printf("-----------------------------------\n");
    fclose(file);
}

// ===== Main Menu =====
int main() {
    int choice;

    loadDataFromFile();

    do {
        printf("\n===== AIRLINE BOOKING MENU =====\n");
        printf("1. Display Seat Map\n");
        printf("2. Book a Seat\n");
        printf("3. Display All Passengers\n");
        printf("4. Cancel Booking\n");
        printf("5. Search Passenger\n");
        printf("6. View Airline Rules & Regulations\n");
        printf("7. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displaySeats(); break;
            case 2: bookSeat(); break;
            case 3: displayAllPassengers(); break;
            case 4: cancelBooking(); break;
            case 5: searchForPassenger(); break;
            case 6: viewAirlineRules(); break;
            case 7:
                saveDataToFile();
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 7);

    return 0;
}

// ===== Passenger Functions =====
void addPassenger(char id[], char name[], char email[], char phone[], char seatNumber[]) {
    Passenger *newPassenger = (Passenger *)malloc(sizeof(Passenger));
    strcpy(newPassenger->id, id);
    strcpy(newPassenger->name, name);
    strcpy(newPassenger->email, email);
    strcpy(newPassenger->phone, phone);
    strcpy(newPassenger->seatNumber, seatNumber);
    newPassenger->next = NULL;

    if (head == NULL) {
        head = newPassenger;
    } else {
        Passenger *temp = head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newPassenger;
    }
    printf("Passenger %s added successfully for seat %s.\n", name, seatNumber);
}

Passenger* searchPassengerByName(char name[]) {
    Passenger *temp = head;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) 
        return temp;
        temp = temp->next;
    }
    return NULL;
}

Passenger* searchPassengerBySeat(char seatNumber[]) {
    Passenger *temp = head;
    while (temp != NULL) {
        if (strcmp(temp->seatNumber, seatNumber) == 0) 
        return temp;
        temp = temp->next;
    }
    return NULL;
}

void displayAllPassengers() {
    Passenger *temp = head;
    if (!temp) {
        printf("No passengers booked yet.\n");
        return;
    }
    printf("\n--- Passenger List ---\n");
    while (temp) {
        printf("Name: %s | ID: %s | Email: %s | Phone: %s | Seat: %s\n",
               temp->name, temp->id, temp->email, temp->phone, temp->seatNumber);
        temp = temp->next;
    }
    printf("----------------------\n");
}

int getSearchChoice() {
    int choice;
    printf("Search by: 1. Name 2. Seat Number: ");
    scanf("%d", &choice);
    return choice;
}

void searchForPassenger() {
    if (!head) {
        printf("No passengers booked yet.\n");
        return;
    }
    int choice = getSearchChoice();
    if (choice == 1) {
        char name[MAX_NAME];
        printf("Enter passenger name: ");
        scanf(" %[^\n]", name);
        Passenger *found = searchPassengerByName(name);
        if (found)
            printf("Passenger found: %s | ID: %s | Email: %s | Phone: %s | Seat: %s\n",
                   found->name, found->id, found->email, found->phone, found->seatNumber);
        else
            printf("Passenger not found.\n");
    } else if (choice == 2) {
        char seatNumber[10];
        printf("Enter seat number: ");
        scanf("%s", seatNumber);
        Passenger *found = searchPassengerBySeat(seatNumber);
        if (found)
            printf("Passenger found: %s | ID: %s | Email: %s | Phone: %s | Seat: %s\n",
                   found->name, found->id, found->email, found->phone, found->seatNumber);
        else
            printf("Passenger not found.\n");
    } else {
        printf("Invalid choice.\n");
    }
}

// ===== Booking & Cancellation =====
void bookSeat() {
    int row, col;
    char name[MAX_NAME], email[MAX_EMAIL], phone[MAX_PHONE], id[MAX_ID];
    char colLetter;
    char seatNumber[10];

    displaySeats();

    printf("\nEnter seat number (eg. 1a): ");
    if (scanf("%d%c", &row, &colLetter) != 2) {
        printf("Invalid format. Use something like 1a.\n");
        while (getchar() != '\n'); // clear input buffer
        return;
    }

    // Convert letter to uppercase for consistency
    colLetter = toupper(colLetter);

    // Validate row and column
    if (row < 1 || row > ROWS || colLetter < 'A' || colLetter >= 'A' + COLS) {
        printf("Invalid seat selection.\n");
        return;
    }

    // Convert to zero-based indexes
    row--;
    col = colLetter - 'A';

    if (seatMap[row][col] == 1) {
        printf("Sorry, that seat is already booked.\n");
        return;
    }

    // Book the seat
    seatMap[row][col] = 1;
    while (getchar() != '\n');
    printf("Enter passenger ID: ");
    fgets(id, sizeof(id), stdin); id[strcspn(id, "\n")] = '\0';
    printf("Enter passenger name: ");
    fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = '\0';
    printf("Enter passenger phone number: ");
    fgets(phone, sizeof(phone), stdin); phone[strcspn(phone, "\n")] = '\0';
    printf("Enter passenger email: ");
    fgets(email, sizeof(email), stdin); email[strcspn(email, "\n")] = '\0';

    // Build seat number as "RowLetter" format
    sprintf(seatNumber, "%d%c", row + 1, col + 'A');

    addPassenger(id, name, email, phone, seatNumber);
}

void cancelBooking() {
    if (!head) {
        printf("No passengers booked yet.\n");
        return;
    }

    char seatNumber[10];
    printf("Enter seat number to cancel booking (e.g., 2B): ");
    scanf("%s", seatNumber);

    // Find row & col for seat map update
    int row = seatNumber[0] - '0' - 1;
    int col = toupper(seatNumber[1]) - 'A';

    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        printf("Invalid seat number.\n");
        return;
    }

    if (seatMap[row][col] == 0) {
        printf("Seat %s is not booked.\n", seatNumber);
        return;
    }

    seatMap[row][col] = 0;
    if (deletePassenger(seatNumber))
        printf("Booking for seat %s cancelled successfully.\n", seatNumber);
    else
        printf("Passenger not found for seat %s.\n", seatNumber);
}

int deletePassenger(char seatNumber[]) {
    Passenger *temp = head, *prev = NULL;
    while (temp) {
        if (strcmp( temp->seatNumber, seatNumber) == 0) {
            if (prev) prev->next = temp->next;
            else head = temp->next;
            free(temp);
            return 1;
        }
        prev = temp;
        temp = temp->next;
    }
    return 0;
}

// ===== Display Seats =====
void displaySeats() {
    printf("\nSeat Map (0 = Available, X = Booked):\n\n    ");

    // Print column headers as letters (A, B, C, ...)
    for (int j = 0; j < COLS; j++) {
        printf(" %c ", 'A' + j);  
    }
    printf("\n");

    // Print each row
    for (int i = 0; i < ROWS; i++) {
        printf(" %2d ", i + 1);  // row numbers aligned
        for (int j = 0; j < COLS; j++) {
            printf(" %c ", seatMap[i][j] ? 'X' : '0');
        }
        printf("\n");
    }
}
