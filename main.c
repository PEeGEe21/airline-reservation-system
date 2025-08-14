#include "include/input_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int seatNumber;
    struct Passenger *next;
} Passenger;

Passenger *head = NULL;
int seatMap[ROWS][COLS]; // 0 = available, 1 = booked

// ===== Function Prototypes =====
void displaySeats();
void bookSeat();
void addPassenger(char id[], char name[], char email[], char phone[], int seatNumber);
Passenger* searchPassengerByName(char name[]);
Passenger* searchPassengerBySeat(int seatNumber);
int deletePassenger(int seatNumber);
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
        fprintf(fp, "%s,%s,%s,%s,%d\n",
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
        sscanf(line, "%49[^,],%49[^,],%49[^,],%19[^,],%d",
               newP->id,
               newP->name,
               newP->email,
               newP->phone,
               &newP->seatNumber);
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
        printf("1. Display Seat Map:\n");
        printf("2. Book a Seat:\n");
        printf("3. Display All Passengers:\n");
        printf("4. Cancel Booking:\n");
        printf("5. Search Passenger:\n");
        printf("6. View Airline Rules & Regulations:\n"); // NEW MENU OPTION
        printf("7. Exit:\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displaySeats(); break;
            case 2: bookSeat(); break;
            case 3: displayAllPassengers(); break;
            case 4: cancelBooking(); break;
            case 5: searchForPassenger(); break;
            case 6: viewAirlineRules(); break; // NEW OPTION
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
void addPassenger(char id[], char name[], char email[], char phone[], int seatNumber) {
    Passenger *newPassenger = (Passenger *)malloc(sizeof(Passenger));
    strcpy(newPassenger->id, id);
    strcpy(newPassenger->name, name);
    strcpy(newPassenger->email, email);
    strcpy(newPassenger->phone, phone);
    newPassenger->seatNumber = seatNumber;
    newPassenger->next = NULL;

    if (head == NULL) {
        head = newPassenger;
    } else {
        Passenger *temp = head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newPassenger;
    }
    printf("Passenger %s added successfully for seat %d.\n", name, seatNumber);
}

Passenger* searchPassengerByName(char name[]) {
    Passenger *temp = head;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

Passenger* searchPassengerBySeat(int seatNumber) {
    Passenger *temp = head;
    while (temp != NULL) {
        if (temp->seatNumber == seatNumber) return temp;
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
        printf("Name: %s | ID: %s | Email: %s | Phone: %s | Seat: %d\n",
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
            printf("Passenger found: %s | ID: %s | Email: %s | Phone: %s | Seat: %d\n",
                   found->name, found->id, found->email, found->phone, found->seatNumber);
        else
            printf("Passenger not found.\n");
    } else if (choice == 2) {
        int seatNumber;
        printf("Enter seat number: ");
        scanf("%d", &seatNumber);
        Passenger *found = searchPassengerBySeat(seatNumber);
        if (found)
            printf("Passenger found: %s | ID: %s | Email: %s | Phone: %s | Seat: %d\n",
                   found->name, found->id, found->email, found->phone, found->seatNumber);
        else
            printf("Passenger not found.\n");
    } else {
        printf("Invalid choice.\n");
    }
}

// Function to search passenger by ID
Passenger* findPassengerById(const char *id) {
    Passenger *current = head;
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            return current; // Found a passenger with same ID
        }
        current = current->next;
    }
    return NULL; // Not found
}

// ===== Booking & Cancellation =====
void bookSeat() {
    int row, col;
    char name[MAX_NAME], email[MAX_EMAIL], phone[MAX_PHONE], id[MAX_ID];

    displaySeats();

    printf("\nEnter row number (1 - %d): ", ROWS);
    scanf("%d", &row);
    printf("Enter seat number (1 - %d): ", COLS);
    scanf("%d", &col);

    if (row < 1 || row > ROWS || col < 1 || col > COLS) {
        printf("Invalid seat selection.\n");
        return;
    }

    row--; col--;
    if (seatMap[row][col] == 1) {
        printf("Sorry, that seat is already booked.\n");
        return;
    }

    while (getchar() != '\n'); // Clear input buffer
    printf("Enter passenger ID: ");
    fgets(id, sizeof(id), stdin); id[strcspn(id, "\n")] = '\0';

    //  Check for duplicate ID before booking
    if (findPassengerById(id) != NULL) {
        printf("Error: Passenger with this ID already exists. Cannot book.\n");
        return;
    }

    printf("Enter passenger name: ");
    fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = '\0';
    printf("Enter passenger phone number: ");
    fgets(phone, sizeof(phone), stdin); phone[strcspn(phone, "\n")] = '\0';
    printf("Enter passenger email: ");
    fgets(email, sizeof(email), stdin); email[strcspn(email, "\n")] = '\0';

    seatMap[row][col] = 1; // Mark seat as booked
    int seatNumber = row * COLS + col + 1;
    addPassenger(id, name, email, phone, seatNumber);

    printf("Seat booked successfully!\n");
}

void cancelBooking() {
    if (!head) {
        printf("No passengers booked yet.\n");
        return;
    }
    int seatNumber;
    printf("Enter seat number to cancel booking: ");
    scanf("%d", &seatNumber);

    if (seatNumber < 1 || seatNumber > TOTAL_SEATS) {
        printf("Invalid seat number.\n");
        return;
    }

    int row = (seatNumber - 1) / COLS;
    int col = (seatNumber - 1) % COLS;

    if (seatMap[row][col] == 0) {
        printf("Seat %d is not booked.\n", seatNumber);
        return;
    }

    seatMap[row][col] = 0;
    if (deletePassenger(seatNumber))
        printf("Booking for seat %d cancelled successfully.\n", seatNumber);
    else
        printf("Passenger not found for seat %d.\n", seatNumber);
}

int deletePassenger(int seatNumber) {
    Passenger *temp = head, *prev = NULL;
    while (temp) {
        if (temp->seatNumber == seatNumber) {
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
    for (int j = 0; j < COLS; j++) printf(" %d ", j + 1);
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        printf("  %d ", i + 1);
        for (int j = 0; j < COLS; j++) {
            printf(" %c ", seatMap[i][j] ? 'X' : '0');
        }
        printf("\n");
    }
}
