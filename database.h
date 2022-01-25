/* Project header for interacting with sqlite3 database. */
#ifndef DATABASE_H_
#define DATABASE_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <sqlite3.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#define bool int
#define true 1
#define false 0


// Get database of specified dbname
sqlite3 *getDatabase(char dbname[]);
// Checks if user exists using query
bool userExists(char dbname[], char username[]);
// Authenticate user
bool authenticate(char dbname[], char username[], char password[]);
// Create new user using credentials
bool createUser(char dbname[], char username[], char password[]);
// Get friends of specified user
void fetchFriends(char dbname[], char user[], char *response);
// Add message from sender to receiver to database
void addMessage(char dbname[], char sender[], char receiver[], char content[]);
// Fetch messages between users
void fetchMessages(char dbname[], char sender[], char receiver[], int client);
// Create session for user after login
bool createSession(char dbname[], char username[], int client);
// Remove session for user after logout
void removeSession(char dbname[], int client);
// Add new friend to database
void createFriend(char dbname[], char username[], char friend[]);

#endif
