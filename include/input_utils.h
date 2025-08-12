#ifndef INPUT_UTILS_H
#define INPUT_UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
typedef char* string;
// Returns a pointer to buffer containing the input string.
string get_string(const char *question);
int get_int(const char *question);
float get_float(const char *question);
double get_double(const char *question);
char get_char(const char *question);
bool get_bool(const char *question);


// Reads a line from stdin into buffer, up to max_len-1 characters.
// Returns true if successful, false on error or EOF.
static inline bool read_line(char *buffer, size_t max_len) {
    if (fgets(buffer, (int)max_len, stdin) == NULL) {
        return false;
    }
    // Remove trailing newline if present
    size_t len = 0;
    while (buffer[len] != '\0') {
        if (buffer[len] == '\n') {
            buffer[len] = '\0';
            break;
        }
        len++;
    }
    return true;
}

// Reads an integer from stdin into *value.
// Returns true if successful, false otherwise.
static inline bool read_int(int *value) {
    char buffer[64];
    if (!read_line(buffer, sizeof(buffer))) {
        return false;
    }
    return sscanf(buffer, "%d", value) == 1;
}

#endif // INPUT_UTILS_H