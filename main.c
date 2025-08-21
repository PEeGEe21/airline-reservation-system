#include "include/input_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_NAME 50
#define MAX_PHONE 20
#define MAX_EMAIL 50
#define MAX_ID 50  
#define ROWS 5
#define COLS 6
#define TOTAL_SEATS (ROWS * COLS)
#define MAX_ADMINS 10
#define MAX_USERNAME 30
#define MAX_PASSWORD 30

// STRUCT TO STORE PASSENGER DETAILS
typedef struct Passenger {
    char id[MAX_ID];     
    char name[MAX_NAME]; 
    char email[MAX_EMAIL];
    char phone[MAX_PHONE]; 
    int seatNumber;
    struct Passenger *next;
} Passenger;

// ===== Admin Struct =====
typedef struct Admin {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
} Admin;

Passenger *head = NULL;
int seatMap[ROWS][COLS]; // 0 = available, 1 = booked
Admin admins[MAX_ADMINS];
int adminCount = 0;

// ===== Function Prototypes =====
void displaySeats();
void bookSeat();
void addPassenger(char id[], char name[], char email[], char phone[], int seatNumber);
Passenger* searchPassengerByName(char name[]);
Passenger* searchPassengerBySeat(int seatNumber);
Passenger* findPassengerById(const char *id);
int deletePassenger(int seatNumber);
char confirmDeletion();
void displayAllPassengers();
void searchForPassenger();
int getSearchChoice();
void cancelBooking();
void saveDataToFile();
void loadDataFromFile();
void viewAirlineRules();
void cancelUserBooking();
void userMenu();
void searchForMyPassengerDetails();

// ===== Admin Functions =====
int loginAdmin();
void createNewAdmin();
void adminMenu();
void saveAdminData();
void loadAdminData();
void displayAdmins();

// ===== File Handling =====
void saveDataToFile() {
    // Step 1: Load the current file into a temporary seat map
    FILE *fp = fopen("booking_data.txt", "r");
    if (fp) {
        int fileRows, fileCols;
        fscanf(fp, "%d %d", &fileRows, &fileCols);

        int tempSeatMap[ROWS][COLS];

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                fscanf(fp, "%d", &tempSeatMap[i][j]);
            }
        }

        fclose(fp);

        // Step 2: Compare tempSeatMap with in-memory seatMap
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                // If the file says it's booked (1) but we think it's free (0),
                // it means a conflict → abort save
                if (tempSeatMap[i][j] == 1 && seatMap[i][j] == 0) {
                    printf("Conflict detected: seat [%d,%d] was booked by someone else. Aborting save!\n", i, j);
                    return;
                }
            }
        }
    }

    // Step 3: Proceed with saving (overwrite the file safely)
    fp = fopen("booking_data.txt", "w");
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

    // Save passengers linked list
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

    // Step 4: Print success timestamp
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    printf("Data saved successfully at %02d:%02d:%02d on %02d/%02d/%d\n",
           local->tm_hour, local->tm_min, local->tm_sec,
           local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
}


void saveAdminData() {
    FILE *fp = fopen("admin_data.txt", "w");
    if (!fp) {
        printf("Error saving admin data!\n");
        return;
    }
    fprintf(fp, "%d\n", adminCount);
    for (int i = 0; i < adminCount; i++) {
        fprintf(fp, "%s,%s\n", admins[i].username, admins[i].password);
    }
    fclose(fp);
    printf("Admin data saved successfully!\n");
}


// load data from file
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

// load data from file
void loadAdminData() {
    FILE *fp = fopen("admin_data.txt", "r");
    if (!fp) {
        printf("No admin data found. Using default admin.\n");
        return;
    }

    fscanf(fp, "%d", &adminCount);
    
    if (adminCount > MAX_ADMINS) {
        printf("Warning: Too many admins in file. Loading first %d only.\n", MAX_ADMINS);
        adminCount = MAX_ADMINS;
    }

    char dummy;
    fscanf(fp, "%c", &dummy);
    
    char line[200];
    for (int i = 0; i < adminCount && fgets(line, sizeof(line), fp); i++) {
        line[strcspn(line, "\n")] = '\0';
        
        char *token = strtok(line, ",");
        if (token != NULL) {
            strncpy(admins[i].username, token, MAX_USERNAME - 1);
            admins[i].username[MAX_USERNAME - 1] = '\0';
            
            token = strtok(NULL, ",");
            if (token != NULL) {
                strncpy(admins[i].password, token, MAX_PASSWORD - 1);
                admins[i].password[MAX_PASSWORD - 1] = '\0';
            }
        }
    }

    fclose(fp);
    printf("Admin data loaded successfully! (%d admins loaded)\n", adminCount);
}

// ===== GET AIRLINE RULES =====
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


// ===== Admin System =====
int loginAdmin() {
    char username[MAX_USERNAME], password[MAX_PASSWORD];

    printf("Enter admin username: ");
    scanf("%s", username);
    printf("Enter admin password: ");
    scanf("%s", password);

    for (int i = 0; i < adminCount; i++) {
        if (strcmp(admins[i].username, username) == 0 &&
            strcmp(admins[i].password, password) == 0) {
            printf("Login successful!\n");
            return 1; 
        }
    }
    printf("Invalid admin credentials.\n");
    return 0; 
}


void createNewAdmin() {
    if (adminCount >= MAX_ADMINS) {
        printf("Max admin limit reached!\n");
        return;
    }
    printf("Enter new admin username: ");
    scanf("%s", admins[adminCount].username);
    printf("Enter new admin password: ");
    scanf("%s", admins[adminCount].password);
    adminCount++;

    saveAdminData();

    printf("New admin created successfully!\n");
}

// ===== Admin Menu =====
void adminMenu() {
    int choice;
    do {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Display Seat Map\n");
        printf("2. Book a Seat\n");
        printf("3. Display All Passengers\n");
        printf("4. Cancel Booking\n");
        printf("5. Search Passenger\n");
        printf("6. View Airline Rules & Regulations\n");
        printf("7. Save Data Now\n");
        printf("8. Create a New Admin\n");
        printf("9. Show All Admin\n");
        printf("0. Exit Admin Menu\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: displaySeats(); break;
            case 2: bookSeat(); break;
            case 3: displayAllPassengers(); break;
            case 4: cancelBooking(); break;
            case 5: searchForPassenger(); break;
            case 6: viewAirlineRules(); break;
            case 7: saveDataToFile(); break;
            case 8: createNewAdmin(); break;
            case 9: displayAdmins(); break;
            case 0: printf("Exiting Admin Menu...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);
}

// ===== User Menu =====
void userMenu() {
    int choice;
    do {
        printf("\n===== USER MENU =====\n");
        printf("1. Display Seat Map\n");
        printf("2. Book a Seat\n");
        printf("3. Cancel My Booking\n");
        printf("4. Search My Booking Detail\n");
        printf("5. View Airline Rules & Regulations\n");
        // printf("6. Save Data Now\n");
        printf("6. Exit User Menu\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: displaySeats(); break;
            case 2: bookSeat(); break;
            case 3: cancelUserBooking(); break;
            case 4: searchForMyPassengerDetails(); break;
            case 5: viewAirlineRules(); break;
            // case 6: saveDataToFile(); break;
            case 6: printf("Exiting User Menu...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 6);
}

// ===== Main Menu=====
int main() {
    loadDataFromFile();
    loadAdminData();

    int roleChoice;
    while (1) {  
        printf("\n===== AIRLINE RESERVATION SYSTEM =====\n");
        printf("1. Admin Login\n");
        printf("2. User Access\n");
        printf("3. Exit\n");
        printf("Choose role: ");
        scanf("%d", &roleChoice);

        switch(roleChoice){
            case 1: 
                if (loginAdmin()) {
                    adminMenu();
                } else {
                    continue;
                }
                break;
            case 2: userMenu(); break;
            case 3: printf("Exiting system...\n"); 
                return 0;
            default:
                 printf("Invalid choice. Try again.\n");
                 break;

        }
    }

    // saveDataToFile();
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

    saveDataToFile();

}

// search all passengers by name
Passenger* searchPassengerByName(char name[]) {
    // use linear search to search for passenger by name
    Passenger *temp = head;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

// search all passengers by seat
Passenger* searchPassengerBySeat(int seatNumber) {
    Passenger *temp = head;
    // use linear search to search for passenger by seatNumber
    while (temp != NULL) {
        if (temp->seatNumber == seatNumber) return temp;
        temp = temp->next;
    }
    return NULL;
}

// display all passengers
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

// ===== Display Admins =====
void displayAdmins() {
    if (adminCount == 0) {
        printf("No admins registered yet.\n");
        return;
    }

    printf("\n--- Admin List ---\n");
    for (int i = 0; i < adminCount; i++) {
        printf("%d. Username: %s | Password: %s\n",
               i + 1,
               admins[i].username,
               admins[i].password);
    }
    printf("------------------\n");
}

// search choice
int getSearchChoice() {
    int choice;
    printf("Search by: 1. Name 2. Seat Number: ");
    scanf("%d", &choice);
    return choice;
}

// confirm delete
char confirmDeletion() {
    char choice;
    printf("Are you sure you want to cancel this booking? (y/n): \n" );
    scanf("%s", &choice);
    return choice;
}

// search passenger
void searchForPassenger() {
    if (!head) {
        printf("No passengers booked yet.\n");
        return;
    }

    // get search choice
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

void searchForMyPassengerDetails() {
    if (!head) {
        printf("No passengers booked yet.\n");
        return;
    }

    char id[MAX_ID];
    printf("Enter your passenger ID to cancel booking: ");
    scanf("%s", id);

    // find passenger by ID
    Passenger *p = findPassengerById(id);
    if (!p) {
        printf("No booking found for passenger ID: %s\n", id);
        return;
    }
    
    printf("We found your booking:\n");
    printf("Name: %s | Email: %s | Phone: %s | Seat: %d\n",
        p->name, p->email, p->phone, p->seatNumber);
}

// Function to search passenger by ID
Passenger* findPassengerById(const char *id) {
    Passenger *current = head;
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            return current;
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
    // scanf("%d", &row);
    if (scanf(" %d", &row) != 1 || row < 1 || row > ROWS) {
        printf("Invalid row number.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter seat number (1 - %d): ", COLS);
    if (scanf(" %d", &col) != 1 || col < 1 || col > COLS) {
        printf("Invalid seat number.\n");
        while (getchar() != '\n');
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

// cancel booking
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

    char confirmDelete = confirmDeletion();

    // confirm delete
    if(confirmDelete == 'y' || confirmDelete == 'Y' ){
        seatMap[row][col] = 0;
        if (deletePassenger(seatNumber))
            printf("Booking for seat %d cancelled successfully.\n", seatNumber);
        else
            printf("Passenger not found for seat %d.\n", seatNumber);
    }
    return;
}

void cancelUserBooking() {
    if (!head) {
        printf("No passengers booked yet.\n");
        return;
    }

    char id[MAX_ID];
    printf("Enter your passenger ID to cancel booking: ");
    scanf("%s", id);

    // find passenger by ID
    Passenger *p = findPassengerById(id);
    if (!p) {
        printf("No booking found for passenger ID: %s\n", id);
        return;
    }

    int seatNumber = p->seatNumber;
    int row = (seatNumber - 1) / COLS;
    int col = (seatNumber - 1) % COLS;

    printf("We found your booking:\n");
    printf("Name: %s | Email: %s | Phone: %s | Seat: %d\n",
           p->name, p->email, p->phone, p->seatNumber);

    char confirmDelete = confirmDeletion();

    if (confirmDelete == 'y' || confirmDelete == 'Y') {
        seatMap[row][col] = 0; // free the seat
        if (deletePassenger(seatNumber)) {
            printf("Booking for seat %d cancelled successfully.\n", seatNumber);
        } else {
            printf("Error: Passenger not found during deletion.\n");
        }
    } else {
        printf("Cancellation aborted.\n");
    }
}


// delete passenger
int deletePassenger(int seatNumber) {
    Passenger *temp = head, *prev = NULL;
    // use linear search to search passenger
    while (temp) {
        if (temp->seatNumber == seatNumber) {
            if (prev) prev->next = temp->next;
            else head = temp->next;
            free(temp);
            saveDataToFile();
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
