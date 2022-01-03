// Implementation of database functions to work on sqlite3 database
#include "database.h"


/*
    Get database of dbname.
    If it doesn't exist, empty database of specified name
    will be automatically created.
*/
sqlite3 *getDatabase(char dbname[]) {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open(dbname, &db);
    if (rc) {
        printf("Can't open the database!\n");
    }
    printf("Database opened.\n");
    
    return db;
}


/*
    Callback for all SQL statement executions.
*/
static int userCallback(void *data, int argc, char **argv, char **azColName) {
    // Callback is used when row can be fetched, which means the user exists
    char **resp = (char **)data;
    strcpy(*resp, "true");
    return 0;
}


/*
    Verifies if the user exists in the database
*/
bool userExists(char dbname[], char query[]) {
    sqlite3 *db = getDatabase(dbname);
    char *zErrMsg = 0;
    char *resp = (char*)malloc(7);
    strcpy(resp, "false");
    int rc;
    bool exists = false;

    rc = sqlite3_exec(db, query, userCallback, &resp, &zErrMsg);

    if( rc != SQLITE_OK ){
        printf("SQL error while checking if user exists: %s\n", zErrMsg);
    }
    else {
        if (strcmp(resp, "true") == 0) exists = true;
    }
    sqlite3_close(db);
    sqlite3_free(zErrMsg);
    free(resp);
    return exists;
}


/*
    Creates new user in database using provided username
    and password. Returns 1 if successful or 0 if there was
    and error.
*/
bool createUser(char dbname[], char username[], char password[]) {
    sqlite3 *db = getDatabase(dbname);
    char *zErrMsg = 0;
    int rc;
    bool created = true;
    char query[100 + strlen(username) + strlen(password)];
    
    strcpy(query, "INSERT INTO users (username, password) VALUES ('");
    strcat(query, username);
    strcat(query, "', '");
    strcat(query, password);
    strcat(query, "');");

    rc = sqlite3_exec(db, query, 0, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        printf("SQL error while creating new user: %s\n", zErrMsg);
        created = false;
    }
    sqlite3_close(db);
    sqlite3_free(zErrMsg);
    return created;
}
