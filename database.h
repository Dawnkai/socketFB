/* Project header for interacting with sqlite3 database. */
#ifndef DATABASE_H_
#define DATABASE_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <sqlite3.h>

#define bool int
#define true 1
#define false 0


// Get database of specified dbname
sqlite3 *getDatabase(char dbname[]);
// Checks if user exists using query
bool userExists(char dbname[], char query[]);
// Create new user using credentials
bool createUser(char dbname[], char username[], char password[]);

#endif
