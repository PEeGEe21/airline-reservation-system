#include "input_utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
typedef char* string;

string get_string(const char *question) {
    static char buffer[256];
    printf("%s", question);
    if (fgets(buffer, sizeof(buffer), stdin)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        return buffer;
    }
    return NULL;
}

int get_int(const char *question) {
    char buffer[100];
    int value;
    while (1) {
        printf("%s", question);
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (sscanf(buffer, "%d", &value) == 1) {
                return value;
            }
        }
        printf("Invalid input. Please enter an integer.\n");
    }
}

float get_float(const char *question) {
    char buffer[100];
    float value;
    while (1) {
        printf("%s", question);
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (sscanf(buffer, "%f", &value) == 1) {
                return value;
            }
        }
        printf("Invalid input. Please enter a float.\n");
    }
}

double get_double(const char *question) {
    char buffer[100];
    double value;
    while (1) {
        printf("%s", question);
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (sscanf(buffer, "%lf", &value) == 1) {
                return value;
            }
        }
        printf("Invalid input. Please enter a double.\n");
    }
}

char get_char(const char *question) {
    char buffer[100];
    while (1) {
        printf("%s", question);
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (strlen(buffer) == 2 && buffer[1] == '\n') {
                return buffer[0];
            }
        }
        printf("Invalid input. Please enter a single character.\n");
    }
}

bool get_bool(const char *question) {
    char buffer[100];
    while (1) {
        printf("%s (y/n): ", question);
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (tolower(buffer[0]) == 'y') return true;
            if (tolower(buffer[0]) == 'n') return false;
        }
        printf("Invalid input. Please enter 'y' or 'n'.\n");
    }
}