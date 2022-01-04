# socketFB
Multi client text communicator using BSD-Sockets with server written in C and client in Python.
Server and client uses multi-threading. GUI is written in KivyMD.

## Contents

* Makefile - make file for easy compilation
* client.py - Python client (**without GUI**) template for connecting to server and testing
* database.h - Header file with definitions for database managment
* database.c - Functions used by server to connect to sqlite3 database and fetch data from it
* parser.h - Header file for server parser
* parser.c - Function parsing user requests
* server.c - Server source code
* utils.h - Header file with server definitions
* utils.c - Functions used by server to handle requests from client

## Compilation

You can easily compile this project using makefile. Usage depends on how you wish to import sqlite3 library.

### Using sqlite3 amalgamation
1. Download latest amalgamation in .zip format
2. Unpack it's contents (sqlite3 and shell) into repository
3. `make build_full`

### Using sqlite3 library
1. `sudo apt-get install libsqlite3-dev`
2. `make build_nosql`


> Once you've built the sqlite3 files you can compile server only using `make build` command.

## Database format

Database needs to be in correct format in order to run. The server will create the database if it doesn't exist on its own, but it will not create the structure for the user (as this is kind of beyond what the server is supposed to do anyway). In order to create or edit database use the following command:

```
sqlite3 <dbname>
```

> Commands used to create database are stored in script.sql file.

### Users
Users are stored in `users` table. It consists of self-incrementing `id`, `username` and `password`.

### Messages
Messages are stored in `messages` table. It consists of self-incrementing `id`, `sender` and `receiver`.

### Friends
Friends are stored in `friends` table. It consists of self-incrementing `id`, `user` and `friend`.

## Authors

* [DawnKai (Maciej Kleban)](https://github.com/Dawnkai/)
