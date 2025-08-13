#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// STRUCT TO STORE PASSENGER DETAILS

typedef struct Passenger {
    char name[50];
    char id[20];
    char contact[20];
    int seatNumber;

struct Passenger *next; // Pointer to next passenger in the linked list
} Passenger;

// Head of the linked list (starts empty)
Passenger *head = NULL;


// FUNCTION: Add Passenger

void addPassenger(char name[], char id[], char contact[], int seatNumber) {
    // Create a new passenger node
    Passenger *newPassenger = (Passenger *)malloc(sizeof(Passenger));

    // Copy data into the new node
    strcpy(newPassenger->name, name);
    strcpy(newPassenger->id, id);
    strcpy(newPassenger->contact, contact);
    newPassenger->seatNumber = seatNumber;

    newPassenger->next = NULL;

    // If the list is empty, new passenger becomes the head
    if (head == NULL) {
        head = newPassenger;
    } else {
        // Otherwise, find the last passenger and link it
        Passenger *temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newPassenger;
    }
    printf("Passenger %s added successfully for seat %d.\n", name, seatNumber);
}

// FUNCTION: Search Passenger by Name

Passenger* searchPassengerByName(char name[]) {
    Passenger *temp = head;

    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            return temp; // Found
        }
        temp = temp->next;
    }
    return NULL; // Not found
}

// FUNCTION: Search Passenger by Seat Number

Passenger* searchPassengerBySeat(int seatNumber) {
    Passenger *temp = head;

    while (temp != NULL) {
        if (temp->seatNumber == seatNumber) {
            return temp; // Found
        }
        temp = temp->next;
    }
    return NULL; // Not found
}

// FUNCTION: Delete Passenger

int deletePassenger(int seatNumber) {
    Passenger *temp = head;
    Passenger *prev = NULL;

    while (temp != NULL) {
        if (temp->seatNumber == seatNumber) {
            // If passenger is the first in the list
            if (prev == NULL) {
                head = temp->next;
            } else {
                prev->next = temp->next;
            }

            free(temp); // Free the memory
            return 1; // Successfully deleted
        }
        prev = temp;
        temp = temp->next;
    }
    return 0; // Passenger not found
}


// FUNCTION: Display All Passengers

void displayAllPassengers() {
    Passenger *temp = head;

    if (temp == NULL) {
        printf("No passengers booked yet.\n");
        return;
    }

    printf("\n--- Passenger List ---\n");
    while (temp != NULL) {
        printf("Name: %s | ID: %s | Contact: %s | Seat: %d\n",
               temp->name, temp->id, temp->contact, temp->seatNumber);
        temp = temp->next;
    }
    printf("----------------------\n");
}


// DEMO MAIN (for testing Part 2 only)

int main() {
    // Adding passengers
    addPassenger("Opeyemi John", "P001", "1234567890", 5);
    addPassenger("Jethro Opeyemi", "P002", "0987654321", 8);

    // Display all passengers
    displayAllPassengers();

    // Search by name
    Passenger *found = searchPassengerByName("John Doe");
    if (found != NULL) {
    printf("\nFound Passenger: %s (Seat %d)\n", found->name, found->seatNumber);
    }

    // Delete passenger
    if (deletePassenger(5)) {
    printf("\nPassenger with seat 5 deleted.\n");
    }

    // Display after deletion
    displayAllPassengers();

    return 0;
}
