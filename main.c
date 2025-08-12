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

// load passenger data

// save passenger data

// find passenger by seat

// find passenger by name

// book a seat

// cancel a booking

// display seat map

// list all passengers

// print menu

// main function
int main(void) {
    print("Welcome to the Airline Reservation System!\n");
    return 0;
}