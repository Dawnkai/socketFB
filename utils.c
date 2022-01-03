// Implementation of utility functions defined in utils.h
#include "utils.h"


/*
    Handles client connection in separate thread and processes
    data sent by client.
*/
void* handleClient(void *arg) {
    /*
        We assume no request is longer than 4096 characters, otherwise
        we would need to expand memory infinitely using realloc, which is
        potentially unsafe.
    */ 
    char request[4096];
    // No endpoint should be longer than 1024 characters
    char endpoint[1024];
    char data[4096];
    printf("New client connected! \n");
    int client = *((int *)arg);
    int msg_status;
    int i = 0;
    int j;
    // Infinite loop
    for(;;) {
        // FIXME: Potential lack of break conditions
        msg_status=recv(client, request, 4096, 0);
        if (msg_status >= 1) {
            // Copy endpoint to another char array
            while (request[i] != ' ') {
                endpoint[i] = request[i];
                i++;
            }
            // If endpoint is not specified, ignore the request
            if (strlen(endpoint) != 0) {
                // Copy rest of the data to separate buffer
                for(j = i + 1; j < strlen(request); j++) data[j - (i + 1)] = request[j];
                parseRequest(data, endpoint, client);
            }
            // Reset variables before another request
            endpoint[0] = '\0';
            data[0] = '\0';
            i = 0;
            j = 0;
        }
        else break;
    }
    printf("Client disconnected! \n");
    pthread_exit(NULL);
}


/*
    Extracts type of request sent by user (GET, POST, PUT) on specific endpoint
    and calls appropriate functions with their respective arguments, then returns requested
    data to the client.
*/
void parseRequest(char request[], char endpoint[], int client) {
    char data[4096];
    char *response = (char*)malloc(4096);
    int i;


    if (strlen(request) < 3) strcpy(response, "401 : Bad Request");
    else if (strstr(request, "GET")) {
        for (i = 4; i < strlen(request); i++) data[i-4] = request[i];
        response = get(data, endpoint);
    }
    else if (strstr(request, "POST")) {
        for (i = 5; i < strlen(request); i++) data[i-5] = request[i];
        response = post(data, endpoint);
    }
    else if (strstr(request, "PUT")) {
        for (i = 4; i < strlen(request); i++) data[i-4] = request[i];
        response = put(data, endpoint);
    }
    else strcpy(response, "401: Incorrect method.");

    printf("response is: %s\n", response);

    send(client, response, strlen(response), 0);
    free(response);
}


/*
    Processes GET requests by calling required functions
    based on the endpoint the request was sent to.
*/
char* get(char request[], char endpoint[]) {
    if (strcmp(endpoint, "friends") == 0) return getFriends(request);
    else if (strcmp(endpoint, "messages") == 0) return getMessages(request);
    // TODO: Return 404 : Not Found
    return "404: Endpoint doesn't exist.";
}


/*
    Processes POST requests by calling required functions
    based on the endpoint the request as sent to.
*/
char* post(char request[], char endpoint[]) {
    char *response = (char*)malloc(4096);
    if (strcmp(endpoint, "login") == 0) login(request, response);
    else if (strcmp(endpoint, "send") == 0) sendMessage(request);
    else if (strcmp(endpoint, "signup") == 0) signup(request);
    else strcpy(response, "404: Endpoint doesn't exist.");
    return response;
}


/*
    Processes PUT requests by calling required functions
    based on the endpoint the request as sent to.
*/
char* put(char request[], char endpoint[]) {
    // TODO: If nothing will require update on server, remove this
    return "";
}


/*
    Checks the database if user with specified credentials
    exists (and then logs the user in) or not (and then returns an error).
    Overwrites response attribute.
*/
void login(char credentials[], char *response) {
    struct Credentials res = getCredentials(credentials);
    /* Allocate memory for base SQL query + size of credentials
       Otherwise strcat throws stack smashing error */
    char *query = (char*)malloc(100 + strlen(res.username) + strlen(res.password));
    // Create SQL query
    strcat(query, "SELECT * FROM users WHERE username = '");
    strcat(query, res.username);
    strcat(query, "' AND password = '");
    strcat(query, res.password);
    strcat(query, "';");
    if (userExists(DBNAME, query)) {
        strcpy(response, "200 : Logged in.");
    }
    else strcpy(response, "403 : Credentials incorrect.");
    free(query);
}


/*
    Creates new user in the database with specified credentials
    if he/she doesn't exist already.
*/
char* signup(char credentials[]) {
    return "";
}


/*
    Fetches friends of the specified user (if he/she exists) and 
    returns them based on the params.
*/
char* getFriends(char params[]) {
    return "";
}


/*
    Fetches messages sent from specific user to another user specified
    in params attribute and returns them.
*/
char* getMessages(char params[]) {
    return "";
}


/*
    Sends a message to specific user (if he/she exists).
*/
char* sendMessage(char params[]) {
    return "";
}
