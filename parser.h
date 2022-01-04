/* Lightweight JSON parsing library hardcoded for the project. */
#ifndef PARSER_H_
#define PARSER_H_

#include <string.h>
#include <stdio.h>


struct Credentials {
    char username[4096];
    char password[4096];
};

struct Message {
    char sender[4096];
    char receiver[4096];
    char content[4096];
};

// Extract credentials from input string
struct Credentials getCredentials(char input[]);
// Extract username from input string
void getUser(char input[], char *output);
// Extract components of message string
struct Message getMessage(char input[]);

#endif
