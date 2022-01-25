// Implementation of JSON handling library specified in header file.
#include "parser.h"


// Extract username and password from input
struct Credentials getCredentials(char input[]) {
    char username[4096] = "";
    char password[4096] = "";
    struct Credentials result;
    int i = 2;
    int j = 0;
    int len = strlen(input);
    // Skip "username" key
    while (input[i] != '"' && i < len) i++;
    // Skip '":"' after key skipping is finished
    i = i + 4;
    // Store username
    while (input[i] != '"' && i < len) {
        username[j] = input[i];
        i++;
        j++;
    }
    // Terminate string
    username[j] = '\0';
    i = i + 4;
    // Skip ',"password"' after key skipping is finished
    while (input[i] != '"' && i < len) i++;
    j = 0;
    // Skip '":"' after key skipping is finished
    i = i + 4;
    while (input[i] != '"' && i < len) {
        password[j] = input[i];
        i++;
        j++;
    }
    // Terminate string
    password[j] = '\0';
    // Store result
    strcpy(result.username, username);
    strcpy(result.password, password);
    return result;
}


// Extract username from input
void getUser(char input[], char *output) {
    strcpy(output, "");
    int i = 14;
    while(input[i] != '"') {
        output[i-14] = input[i];
        i++;
    }
    output[i-14] = '\0';
}


// Extract message, sender and receiver from input
struct Message getMessage(char input[]) {
    struct Message msg;
    char tmp[4096] = "";

    // Skip {"
    int pos = 2;
    int i = 0;
    // Skip sender key
    while(input[pos] != '"') pos++;
    // Skip ":"
    pos = pos + 4;
    // Read sender
    while(input[pos] != '"') {
        tmp[i] = input[pos];
        pos++;
        i++;
    }
    tmp[i] = '\0';
    i = 0;
    // Skip ","
    pos = pos + 4;
    strcpy(msg.sender, tmp);
    strcpy(tmp, "");

    // Skip receiver key
    while(input[pos] != '"') pos++;
    // Skip ":"
    pos = pos + 4;
    // Read receiver
    while(input[pos] != '"') {
        tmp[i] = input[pos];
        pos++;
        i++;
    }
    tmp[i] = '\0';
    i = 0;
    // Skip ","
    pos = pos + 4;
    strcpy(msg.receiver, tmp);
    strcpy(tmp, "");

    // Skip content key
    while(input[pos] != '"') pos++;
    // Skip ":"
    pos = pos + 4;
    // Read content
    while(input[pos] != '"') {
        tmp[i] = input[pos];
        pos++;
        i++;
    }
    tmp[i] = '\0';
    strcpy(msg.content, tmp);
    return msg;
}

// Extract receiver and sender from input
struct Message getParticipants(char input[]) {
    struct Message msg;
    char tmp[4096] = "";

    // Skip {"
    int pos = 2;
    int i = 0;
    // Skip sender key
    while(input[pos] != '"') pos++;
    // Skip ":"
    pos = pos + 4;
    // Read sender
    while(input[pos] != '"') {
        tmp[i] = input[pos];
        pos++;
        i++;
    }
    tmp[i] = '\0';
    i = 0;
    // Skip ","
    pos = pos + 4;
    strcpy(msg.sender, tmp);
    strcpy(tmp, "");

    // Skip receiver key
    while(input[pos] != '"') pos++;
    // Skip ":"
    pos = pos + 4;
    // Read receiver
    while(input[pos] != '"') {
        tmp[i] = input[pos];
        pos++;
        i++;
    }
    tmp[i] = '\0';
    strcpy(msg.receiver, tmp);
    strcpy(tmp, "");

    return msg;
}


// Extract username and friend name from input
struct Friend getFriend(char input[]) {
    struct Friend msg;
    char tmp[4096] = "";

    // Skip {"
    int pos = 2;
    int i = 0;
    // Skip username key
    while(input[pos] != '"') pos++;
    // Skip ":"
    pos = pos + 4;
    // Read username
    while(input[pos] != '"') {
        tmp[i] = input[pos];
        pos++;
        i++;
    }
    tmp[i] = '\0';
    i = 0;
    // Skip ","
    pos = pos + 4;
    strcpy(msg.username, tmp);
    strcpy(tmp, "");

    // Skip friend key
    while(input[pos] != '"') pos++;
    // Skip ":"
    pos = pos + 4;
    // Read friend
    while(input[pos] != '"') {
        tmp[i] = input[pos];
        pos++;
        i++;
    }
    tmp[i] = '\0';
    strcpy(msg.friend, tmp);
    strcpy(tmp, "");

    return msg;
}
