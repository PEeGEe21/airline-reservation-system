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

// Seat status: 0 = available, 1 = booked
int seatMap[ROWS][COLS];

void displaySeats();
void bookSeat();
void addPassenger(char name[], char id[], char email[], char contact[], int seatNumber);
Passenger* searchPassengerByName(char name[]);
Passenger* searchPassengerBySeat(int seatNumber);
int deletePassenger(int seatNumber);
void displayAllPassengers();
void searchForPassenger();
int getSearchChoice();
void cancelBooking();


// Main funtion
int main(void) {

    int choice;
    while (1) {
        printf("\n--- Welcome to the Airline Reservation System ---\n");
        printf("1. View Seats\n");
        printf("2. Book a Seat\n");
        printf("3. Cancel booking\n");
        printf("4. Search passenger\n");
        printf("5. Show Passengers\n");
        printf("0. Exit\n");

        printf("Enter choice (eg 1, 2,..): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                displaySeats();
                break;
            case 2:
                bookSeat();
                break;
            case 3:
                printf("Cancel booking\n");
                cancelBooking();
                break;
            case 4:
                searchForPassenger();
                break;
            case 5:
                displayAllPassengers();
                break;
            case 0:
                printf("Exiting program.\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}


// FUNCTION: Add Passenger
void addPassenger(char name[], char id[], char email[], char contact[], int seatNumber) {
    Passenger *newPassenger = (Passenger *)malloc(sizeof(Passenger));
    strcpy(newPassenger->name, name);
    strcpy(newPassenger->email, email);
    strcpy(newPassenger->id, id); 
    strcpy(newPassenger->phone, contact);
    newPassenger->seatNumber = seatNumber;
    newPassenger->next = NULL;

    if (head == NULL) {
        head = newPassenger;
    } else {
        Passenger *temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newPassenger;
    }
    printf("Passenger %s added successfully for seat %d.\n", name, seatNumber);
}

// SearchPassenger Function
Passenger* searchPassengerByName(char name[]) {
    Passenger *temp = head;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// Search Passenger by seat function
Passenger* searchPassengerBySeat(int seatNumber) {
    Passenger *temp = head;
    while (temp != NULL) {
        if (temp->seatNumber == seatNumber) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// Delete Passenger function
void cancelBooking() {
    Passenger *temp = head;

    if (temp == NULL) {
        printf("No passengers booked yet.\n");
        return;
    }
    
    int seatNumber;
    printf("Enter seat number to cancel booking: ");
    if (scanf("%d", &seatNumber) != 1 || seatNumber < 1 || seatNumber > TOTAL_SEATS) {
        printf("Invalid seat number.\n");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }

    int row = (seatNumber - 1) / COLS;
    int col = (seatNumber - 1) % COLS;

    if (seatMap[row][col] == 0) {
        printf("Seat %d is not booked.\n", seatNumber);
        return;
    }

    seatMap[row][col] = 0;

    if (deletePassenger(seatNumber)) {
        printf("Booking for seat %d cancelled successfully.\n", seatNumber);
    } else {
        printf("Passenger not found for seat %d.\n", seatNumber);
    }
}

int deletePassenger(int seatNumber) {
    Passenger *temp = head;
    Passenger *prev = NULL;

    while (temp != NULL) {
        if (temp->seatNumber == seatNumber) {
            if (prev == NULL) {
                head = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            return 1;
        }
        prev = temp;
        temp = temp->next;
    }
    return 0;
}

 // Display all Passenger  function
void displayAllPassengers() {
    Passenger *temp = head;

    if (temp == NULL) {
        printf("No passengers booked yet.\n");
        return;
    }

    printf("\n--- Passenger List ---\n");
    while (temp != NULL) {
        printf("Name: %s | ID: %s | Email: %s | Phone: %s | Seat: %d\n",
               temp->name, temp->id, temp->email, temp->phone, temp->seatNumber);
        temp = temp->next;
    }
    printf("----------------------\n");
}

// Bookseat Function
void bookSeat() {
    int row, col;
    char name[MAX_NAME], email[MAX_ID], phone[MAX_PHONE];

    displaySeats();

    printf("\nEnter row number (1 - %d): ", ROWS);
    if (scanf("%d", &row) != 1 || row < 1 || row > ROWS) {
        printf("Invalid row number.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter seat number (1 - %d): ", COLS);
    if (scanf("%d", &col) != 1 || col < 1 || col > COLS) {
        printf("Invalid seat number.\n");
        while (getchar() != '\n');
        return;
    }

    row--;
    col--;

    if (seatMap[row][col] == 0) {
        seatMap[row][col] = 1;

        // Get passenger details
        while (getchar() != '\n'); // Clear input buffer
        printf("Enter passenger name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';

        printf("Enter passenger phone number: ");
        fgets(phone, sizeof(phone), stdin);
        phone[strcspn(phone, "\n")] = '\0';

        printf("Enter passenger email: ");
        fgets(email, sizeof(email), stdin);
        email[strcspn(email, "\n")] = '\0';

        int seatNumber = row * COLS + col + 1;
        addPassenger(name, email, phone, email, seatNumber);

    } else {
        printf("Sorry, that seat is already booked.\n");
    }
}

// Displayseat Funtion 
void displaySeats() {
    printf("\nSeat Map (0 = Available, X = Booked):\n\n");
    printf("    ");
    for (int j = 0; j < COLS; j++) {
        printf(" %d ", j + 1);
    }
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        printf("  %d ", i + 1);
        for (int j = 0; j < COLS; j++) {
            if (seatMap[i][j] == 0) {
                printf(" 0 ");
            } else {
                printf(" X ");
            }
        }
        printf("\n");
    }
}

int getSearchChoice(){
    int choice;
    printf("Search by: 1. Name 2. Seat Number");
    if (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        printf("Invalid choice. Please enter 1 or 2.\n");
        while (getchar() != '\n'); // Clear input buffer
        return getSearchChoice(); // Retry
    }
    return choice;
}

void searchForPassenger(){
    Passenger *temp = head;
    if (temp == NULL) {
        printf("No passengers booked yet.\n");
        return;
    }
    int choice = getSearchChoice();
    switch (choice) {
        case 1: {
            char name[MAX_NAME];
            printf("Enter passenger name: ");
            scanf(" %[^\n]", name);
            Passenger *found = searchPassengerByName(name);
            if (found) {
                printf("Passenger found: %s | ID: %s | Email: %s | Phone: %s | Seat: %d\n",
                       found->name, found->id, found->email, found->phone, found->seatNumber);
            } else {
                printf("Passenger not found.\n");
            }
            break;
        }
        case 2: {
            int seatNumber;
            printf("Enter seat number: ");
            scanf("%d", &seatNumber);
            Passenger *found = searchPassengerBySeat(seatNumber);
            if (found) {
                printf("Passenger found: %s | ID: %s | Email: %s | Phone: %s | Seat: %d\n",
                       found->name, found->id, found->email, found->phone, found->seatNumber);
            } else {
                printf("Passenger not found.\n");
            }
            break;
        }
        default:
            printf("Invalid choice.\n");
    }   
}
