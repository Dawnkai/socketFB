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
    Callback for user fetching SQL statement executions.
*/
static int userCallback(void *data, int argc, char **argv, char **azColName) {
    // Callback is used when row can be fetched, which means the user exists
    bool *resp = (bool *)data;
    *resp = true;
    return 0;
}


/*
    Verifies if the user exists in the database.
*/
bool userExists(char dbname[], char username[]) {
    sqlite3 *db = getDatabase(dbname);
    char *zErrMsg = 0;
    int rc;
    bool exists = false;
    char query[100 + strlen(username)];

    strcpy(query, "SELECT * FROM users WHERE username = '");
    strcat(query, username);
    strcat(query, "';");

    rc = sqlite3_exec(db, query, userCallback, &exists, &zErrMsg);

    if( rc != SQLITE_OK ){
        printf("SQL error while checking if user exists: %s\n", zErrMsg);
    }
    sqlite3_close(db);
    sqlite3_free(zErrMsg);
    return exists;
}


/*
    Authenticate user by comparing his/her username and password to database
    entries.
*/
bool authenticate(char dbname[], char username[], char password[]) {
    sqlite3 *db = getDatabase(dbname);
    char *zErrMsg = 0;
    bool correct = false;
    int rc;
    char query[100 + strlen(username) + strlen(password)];

    strcpy(query, "SELECT * FROM users WHERE username = '");
    strcat(query, username);
    strcat(query, "' AND password = '");
    strcat(query, password);
    strcat(query, "';");

    rc = sqlite3_exec(db, query, userCallback, &correct, &zErrMsg);
    if( rc != SQLITE_OK ){
        printf("SQL error while checking if user exists: %s\n", zErrMsg);
    }
    sqlite3_close(db);
    sqlite3_free(zErrMsg);
    return correct;
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


/*
    Callback for friends fetching SQL statement executions.
*/
static int friendCallback(void *data, int argc, char **argv, char **azColName) {
    char **resp = (char **)data;
    if (strcmp(*resp, "") == 0) {
        strcat(*resp, argv[0]);
    }
    else {
        strcat(*resp, ",");
        strcat(*resp, argv[0]);
    }
    return 0;
}


/*
    Gets friends of specified user from database.
*/
void fetchFriends(char dbname[], char user[], char *response) {
    sqlite3 *db = getDatabase(dbname);
    char *zErrMsg = 0;
    int rc;
    strcpy(response, "x");
    char query[100 + strlen(user)];

    strcpy(query, "SELECT friend FROM friends WHERE username = '");
    strcat(query, user);
    strcat(query, "';");

    rc = sqlite3_exec(db, query, friendCallback, &response, &zErrMsg);

    if( rc != SQLITE_OK ){
        printf("SQL error while fetching user's friends: %s\n", zErrMsg);
    }
    sqlite3_close(db);
    sqlite3_free(zErrMsg);
}


/*
    Add message to the database from sender to receiver.
*/
void addMessage(char dbname[], char sender[], char receiver[], char content[]) {
    sqlite3 *db = getDatabase(dbname);
    char *zErrMsg = 0;
    int rc;
    char query[100 + strlen(sender) + strlen(receiver) + strlen(content)];

    strcpy(query, "INSERT INTO messages (sender, receiver, content) VALUES ('");
    strcat(query, sender);
    strcat(query, "','");
    strcat(query, receiver);
    strcat(query, "','");
    strcat(query, content);
    strcat(query, "');");

    rc = sqlite3_exec(db, query, 0, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        printf("SQL error while adding message to database: %s\n", zErrMsg);
    }
    sqlite3_close(db);
    sqlite3_free(zErrMsg);
}


/*
    Callback for message fetching SQL statement executions.
*/
static int messageCallback(void *data, int argc, char **argv, char **azColName) {
    char resp[4096] = "";
    int *client = (int *)data;
    strcpy(resp, argv[0]);
    printf("Sending %s\n", resp);
    send(*client, resp, 4096, 0);
    return 0;
}


/*
    Get messages between users and send them directly to the client 
    bit by bit, until all messages are retrieved.
*/
void fetchMessages(char dbname[], char sender[], char receiver[], int client) {
    sqlite3 *db = getDatabase(dbname);
    char *zErrMsg = 0;
    int rc;
    char query[100 + strlen(sender) + strlen(receiver)];

    strcpy(query, "SELECT content FROM messages WHERE sender = '");
    strcat(query, sender);
    strcat(query, "' AND receiver = '");
    strcat(query, receiver);
    strcat(query, "';");

    rc = sqlite3_exec(db, query, messageCallback, &client, &zErrMsg);
    if( rc != SQLITE_OK ){
        printf("SQL error while adding message to database: %s\n", zErrMsg);
    }
    sqlite3_close(db);
    sqlite3_free(zErrMsg);
}


/*
    Create session using username and client file descriptor. Since
    client descriptor is always unique, use it as primary key. After
    disconnect properly working server will remove the user from the database
    so it's not a problem.
*/
bool createSession(char dbname[], char username[], int client) {
    sqlite3 *db = getDatabase(dbname);
    char *zErrMsg = 0;
    int rc;
    char query[100 + strlen(username)];
    bool created = true;
    // Max range of int is 10 digits
    char sClient[10] = "";

    // Convert file descriptor integer into string
    sprintf(sClient, "%d", client);
    strcpy(query, "INSERT INTO sessions (username, fd) VALUES ('");
    strcat(query, username);
    strcat(query,"',");
    strcat(query,sClient);
    strcat(query,");");

    rc = sqlite3_exec(db, query, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        printf("SQL error while adding user session to database: %s\n", zErrMsg);
        created = false;
    }
    sqlite3_close(db);
    sqlite3_free(zErrMsg);

    return created;
}


/*
    After the user has disconnected from the server, remove his entry from session
    table, that is his file descriptor and username. Since file descriptor serves
    as primary key and is always unique - use it instead of filtering usernames
    which generates the need to store them somehow and waste memory.
*/
void removeSession(char dbname[], int client) {
    sqlite3 *db = getDatabase(dbname);
    char *zErrMsg = 0;
    int rc;
    char query[100];
    // Max range of int is 10 digits
    char sClient[10] = "";

    // Convert file descriptor integer into string
    sprintf(sClient, "%d", client);
    strcpy(query, "DELETE FROM sessions WHERE fd = '");
    strcat(query, sClient);
    strcat(query, "';");

    rc = sqlite3_exec(db, query, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        printf("SQL error while closing user session to database: %s\n", zErrMsg);
    }
    sqlite3_close(db);
    sqlite3_free(zErrMsg);
}


/*
    Create new friend in the database by adding username of person requesting
    the change and username of their friend.
*/
void createFriend(char dbname[], char username[], char friend[]) {
    sqlite3 *db = getDatabase(dbname);
    char *zErrMsg = 0;
    int rc;
    char query[100];
    strcpy(query, "INSERT INTO friends (username, friend) VALUES ('");
    strcat(query, username);
    strcat(query, "','");
    strcat(query, friend);
    strcat(query, "');");

    rc = sqlite3_exec(db, query, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        printf("SQL error while adding new friend to database: %s\n", zErrMsg);
    }
    sqlite3_close(db);
    sqlite3_free(zErrMsg);
}

static int sessionCallback(void *data, int argc, char **argv, char **azColName) {
    // Callback is used when row can be fetched, which means the user exists
    bool *resp = (bool *)data;
    *resp = true;
    return 0;
}

bool sessionExists(char dbname[], char username[]) {
    sqlite3 *db = getDatabase(dbname);
    char *zErrMsg = 0;
    int rc;
    char query[100 + strlen(username)];
    bool exists = false;
    strcpy(query, "SELECT * FROM sessions WHERE username = '");
    strcat(query, username);
    strcat(query, "';");

    rc = sqlite3_exec(db, query, sessionCallback, &exists, &zErrMsg);
    if( rc != SQLITE_OK ){
        printf("SQL error while adding new friend to database: %s\n", zErrMsg);
    }
    sqlite3_close(db);
    sqlite3_free(zErrMsg);

    return exists;
}

static int friendExistsCallback(void *data, int argc, char **argv, char **azColName) {
    // Callback is used when row can be fetched, which means the user exists
    bool *resp = (bool *)data;
    *resp = true;
    return 0;
}

bool friendExists(char dbname[], char username[], char friend[]) {
    sqlite3 *db = getDatabase(dbname);
    char *zErrMsg = 0;
    int rc;
    char query[100 + strlen(username)];
    bool exists = false;
    strcpy(query, "SELECT * FROM friends WHERE username = '");
    strcat(query, username);
    strcat(query, "' AND friend = '");
    strcat(query, friend);
    strcat(query, "';");

    rc = sqlite3_exec(db, query, friendExistsCallback, &exists, &zErrMsg);
    if( rc != SQLITE_OK ){
        printf("SQL error while adding new friend to database: %s\n", zErrMsg);
    }
    sqlite3_close(db);
    sqlite3_free(zErrMsg);

    return exists;
}
