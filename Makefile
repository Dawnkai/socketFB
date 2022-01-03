CC = gcc
output = server
sql_source = sqlite3.c shell.c
server_source = server.c utils.c sqlite3.o
libs = -pthread -lm

sql:
	$(CC) $(sql_source) $(libs) -ldl -O2 -c sqlite3.c

compile: sql
	$(CC) -o $(output) $(libs) -lsqlite3 $(server_source) -ldl

clean:
	rm *.o $(output)
