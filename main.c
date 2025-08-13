#include "include/input_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_NAME 50
#define MAX_PHONE 20
#define ROWS 5
#define COLS 6
#define TOTAL_SEATS (ROWS * COLS)

typedef struct Passenger {
    int id;
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    int seatNumber;
    struct Passenger *next;
} Passenger;

void displaySeats();
void bookSeat();


// Seat status: 0 = available, 1 = booked
int seatMap[ROWS][COLS];

// load passenger data

// save passenger data

// find passenger by seat

// find passenger by name

// book a seat
void bookSeat() {
    int row, col;

    displaySeats();

    printf("\nEnter row number (1 - %d): ", ROWS);
    if (scanf(" %d", &row) != 1 || row < 1 || row > ROWS) {
        printf("Invalid row number.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter seat number in (1 - %d): ", COLS);
    if (scanf(" %d", &col) != 1 || col < 1 || col > COLS) {
        printf("Invalid seat number.\n");
        while (getchar() != '\n');
        return;
    }

    // Adjust to 0-based index
    row--;
    col--;

    // Check seat availability
    if (seatMap[row][col] == 0) {
        seatMap[row][col] = 1; 
        printf("Seat Row %d, Seat %d booked successfully!\n", row + 1, col + 1);

    } else {
        printf("Sorry, that seat is already booked.\n");
    }
}

// cancel a booking

// display seat map
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

// list all passengers

// print menu

// main function
int main(void) {
    int choice;

    while (1) {
        printf("\n--- Welcome to the Airline Reservation System ---\n");
        printf("1. View Seats\n");
        printf("2. Book a Seat\n");
        printf("3. Exit\n");
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
                printf("Exiting program.\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}