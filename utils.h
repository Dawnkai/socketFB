/* Utility functions managing client requests to save 
 space on server source code and improve readability. */
#ifndef UTILS_H_
#define UTILS_H_

#include "parser.h"
#include "database.h"

#define DBNAME "sk2"


// Handles new client connection.
void* handleClient(void *args);
// Parses the request from the client and returns response.
void parseRequest(char request[], char endpoint[], int client);
// Parses GET requests.
char* get(char request[], char endpoint[], int client);
// Parses POST requests.
char* post(char request[], char endpoint[], int client);
// Checks if the user exists in the database and logs him/her in.
void login(char credentials[], char *response, int client);
// Creates new account for the user if it doesn't exist.
void signup(char credentials[], char *response);
// Fetches friends of client.
void getFriends(char params[], char *response);
// Gets messages from user to client.
void getMessages(char params[], char *response, int client);
// Sends message from client to another user.
void sendMessage(char params[], char *response);
// Adds a new friend to database
void addFriend(char request[], char *response);

#endif
