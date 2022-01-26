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
        msg_status=recv(client, request, 4096, 0);
        if (msg_status >= 1) {
            // Copy endpoint to another char array
            while (request[i] != ' ') {
                endpoint[i] = request[i];
                i++;
            }
            // If endpoint is not specified, ignore the request
            if (strlen(endpoint) != 0) {
                endpoint[i] = '\0';
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
    removeSession(DBNAME, client);
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
    printf("Called endpoint %s\n", endpoint);


    if (strlen(request) < 3) strcpy(response, "401 : Bad Request");
    else if (strstr(request, "GET")) {
        for (i = 4; i < strlen(request); i++) data[i-4] = request[i];
        response = get(data, endpoint, client);
    }
    else if (strstr(request, "POST")) {
        for (i = 5; i < strlen(request); i++) data[i-5] = request[i];
        response = post(data, endpoint, client);
    }
    else strcpy(response, "401 : Incorrect method.");

    printf("response is: %s\n", response);

    send(client, response, strlen(response), 0);
    free(response);
}


/*
    Processes GET requests by calling required functions
    based on the endpoint the request was sent to.
*/
char* get(char request[], char endpoint[], int client) {
    char *response = (char*)malloc(4096);
    if (strcmp(endpoint, "friends") == 0) getFriends(request, response);
    else if (strcmp(endpoint, "messages") == 0) getMessages(request, response, client);
    else strcpy(response, "404 : Endpoint doesn't exist.");
    return response;
}


/*
    Processes POST requests by calling required functions
    based on the endpoint the request as sent to.
*/
char* post(char request[], char endpoint[], int client) {
    char *response = (char*)malloc(4096);
    if (strcmp(endpoint, "login") == 0) login(request, response, client);
    else if (strcmp(endpoint, "send") == 0) sendMessage(request, response);
    else if (strcmp(endpoint, "signup") == 0) signup(request, response);
    else if (strcmp(endpoint, "friends") == 0) addFriend(request, response);
    else strcpy(response, "404 : Endpoint doesn't exist.");
    return response;
}


/*
    Checks the database if user with specified credentials
    exists (and then logs the user in) or not (and then returns an error).
    Overwrites response attribute.

    Credentials should be in form:
    {'username':'user','password':'pass'}
*/
void login(char credentials[], char *response, int client) {
    struct Credentials res = getCredentials(credentials);
    if (authenticate(DBNAME, res.username, res.password)) {
        if (!sessionExists(DBNAME, res.username) && createSession(DBNAME, res.username, client)) {
            strcpy(response, "200 : Logged in.");
        }
        else strcpy(response, "403 : User already logged in.");
    }
    else strcpy(response, "403 : Credentials incorrect.");
}


/*
    Creates new user in the database with specified credentials
    if he/she doesn't exist already.

    Credentials should be in form:
    {'username':'user','password':'pass'}
*/
void signup(char credentials[], char *response) {
    struct Credentials res = getCredentials(credentials);
    if (userExists(DBNAME, res.username)) {
        strcpy(response, "403 : User already exists.");
    }
    else {
        // Create new user
        if (createUser(DBNAME, res.username, res.password)) strcpy(response, "201 : User created.");
        else strcpy(response, "500 : Internal server error.");
    }
}


/*
    Fetches friends of the specified user (if he/she exists) and 
    returns them based on the params.

    Params should be in form:
    {'username': 'nameoftheuser'}
*/
void getFriends(char params[], char *response) {
    // Substring extraction for checking if params can be extracted
    char tmp[4096] = "";
    int i;
    for(i = 2; i < 12; i++) tmp[i-2] = params[i];
    if (strcmp(tmp, "username\":") == 0) {
        getUser(params, tmp);
        fetchFriends(DBNAME, tmp, response);
    }
    else strcpy(response, "");
}


/*
    Fetches messages sent from specific user to another user specified
    in params attribute and returns them BULK BY BULK. It means that
    this function will not return one response, but instead will keep sending
    all messages until all of them are sent!
    Final message is 200 : Messages received.
    The reason for this is because messages between users can be very long
    vastly exceeding 4096 letters limit.

    Params should be in form:
    {'sender':'user','receiver':'other_user'}
*/
void getMessages(char params[], char *response, int client) {
    struct Message participants = getParticipants(params);
    if (userExists(DBNAME, participants.sender) && userExists(DBNAME, participants.receiver)) {
        fetchMessages(DBNAME, participants.sender, participants.receiver, client);
        strcpy(response, "200 : Messages received.");
    }
    else strcpy(response, "404 : Users do not exist.");
}


/*
    Sends a message to specific user (if he/she exists).

    Params should be in form:
    {'sender':'user','receiver':'other_user','content':'something'}
*/
void sendMessage(char params[], char *response) {
    struct Message msg = getMessage(params);
    if (userExists(DBNAME, msg.sender) && userExists(DBNAME, msg.receiver)) {
        addMessage(DBNAME, msg.sender, msg.receiver, msg.content);
        strcpy(response, "200 : Message sent.");
    }
    else strcpy(response, "403 : Incorrect message.");
}


/*
    Creates new friend for user.

    Params should be in form:
    {'username':'user','friend':'other_user'}
*/
void addFriend(char request[], char *response) {
    struct Friend msg = getFriend(request);
    if (userExists(DBNAME, msg.username) && userExists(DBNAME, msg.friend)) {
        createFriend(DBNAME, msg.username, msg.friend);
        strcpy(response, "201 : Friend added.");
    }
    else strcpy(response, "403 : User does not exist.");
}
