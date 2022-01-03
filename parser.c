// Implementation of JSON handling library specified in header file.
#include "parser.h"


// Extract username and password from input
struct Credentials getCredentials(char input[]) {
    char username[4096] = "";
    char password[4096] = "";
    struct Credentials result;
    int i = 2;
    int j = 0;
    int len = strlen(input);
    // Skip "username" key
    while (input[i] != '"' && i < len) i++;
    // Skip '":"' after key skipping is finished
    i = i + 4;
    // Store username
    while (input[i] != '"' && i < len) {
        username[j] = input[i];
        i++;
        j++;
    }
    // Terminate string
    username[j] = '\0';
    i = i + 4;
    // Skip ',"password"' after key skipping is finished
    while (input[i] != '"' && i < len) i++;
    j = 0;
    // Skip '":"' after key skipping is finished
    i = i + 4;
    while (input[i] != '"' && i < len) {
        password[j] = input[i];
        i++;
        j++;
    }
    // Terminate string
    password[j] = '\0';
    // Store result
    strcpy(result.username, username);
    strcpy(result.password, password);
    return result;
}
