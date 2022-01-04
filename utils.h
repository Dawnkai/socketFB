/* Utility functions managing client requests to save 
 space on server source code and improve readability. */
#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <sqlite3.h>

#include "parser.h"
#include "database.h"

#define DBNAME "data.db"


// Handles new client connection.
void* handleClient(void *args);
// Parses the request from the client and returns response.
void parseRequest(char request[], char endpoint[], int client);
// Parses GET requests.
char* get(char request[], char endpoint[]);
// Parses POST requests.
char* post(char request[], char endpoint[]);
// Parses PUT requests.
char* put(char request[], char endpoint[]);
// Checks if the user exists in the database and logs him/her in.
void login(char credentials[], char *response);
// Creates new account for the user if it doesn't exist.
void signup(char credentials[], char *response);
// Fetches friends of client.
void getFriends(char params[], char *response);
// Gets messages from user to client.
char* getMessages(char params[]);
// Sends message from client to another user.
char* sendMessage(char params[]);

#endif
