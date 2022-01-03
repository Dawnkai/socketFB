CC = gcc
output = server
sql_source = sqlite3.c shell.c
sql_output = sqlite3.o
server_source = server.c utils.c parser.h parser.c database.h database.c
libs = -pthread -lm

# Build sqlite3 amalgamation
sql:
	$(CC) $(sql_source) $(libs) -ldl -O2 -c sqlite3.c

# Build the server (with sqlite amalgamation)
build:
	$(CC) -Wall -o $(output) $(libs) -lsqlite3 $(server_source) $(sql_output) -ldl

build_full: sql build

# Build server without sqlite3 amalgamation
build_nosql: 
	$(CC) -Wall -o $(output) $(libs) -lsqlite3 $(server_source) -ldl

clean:
	rm *.o $(output)
