/* Lightweight JSON parsing library hardcoded for the project. */
#ifndef PARSER_H_
#define PARSER_H_

#include <string.h>
#include <stdio.h>


struct Credentials {
    char username[4096];
    char password[4096];
};

// Extract credentials from input string
struct Credentials getCredentials(char input[]);

#endif
