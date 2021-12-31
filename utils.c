// Implementation of utility functions defined in utils.h
#include "utils.h"


/*
    Handles client connection in separate thread and processes
    data sent by client.
*/
void* handleClient(void *arg) {
    char request[1024];
    printf("New client connected! \n");
    int client = *((int *)arg);
    int msg_status;
    for(;;) {
        msg_status=recv(client, request, 1024, 0);
        while (!(msg_status<1)) {
            // TODO: Fetch messages from client and merge them
        }
    }
    printf("Client disconnected! \n");
    pthread_exit(NULL);
}


/*
    Extracts type of request sent by user (GET, POST, PUT) on specific endpoint
    and calls appropriate functions with their respective arguments.
*/
void parseRequest(char request[], char endpoint[]) {
    if (strlen(request) < 3) {
        // TODO: Return 401 : Bad Request
        return;
    }

    char *pos;
    char *data;
    char *response;
    int i;
    if (strstr(request, "GET")) {
        data = (char*)malloc(strlen(request) - 3);
        for (i = 3; i < strlen(request); i++) data[i-3] = request[i];
        response = get(data, endpoint);
        // TODO: Return response
    }
    else if (strstr(request, "POST")) {
        data = (char*)malloc(strlen(request) - 4);
        for (i = 4; i < strlen(request); i++) data[i-4] = request[i];
        response = post(data, endpoint);
        // TODO: Return response
    }
    else if (strstr(request, "PUT")) {
        data = (char*)malloc(strlen(request) - 3);
        for (i = 3; i < strlen(request); i++) data[i-3] = request[i];
        response = put(data, endpoint);
        // TODO: Return response
    }
    else {
        // TODO: Return 401 : Bad Request
        return;
    }
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
    if (strcmp(endpoint, "login") == 0) return login(request);
    else if (strcmp(endpoint, "send") == 0) return sendMessage(request);
    else if (strcmp(endpoint, "signup") == 0) return signup(request);
    // TODO: Return 404 : Not Found
    return "404: Endpoint doesn't exist.";
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
*/
char* login(char credentials[]) {
    return "";
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
