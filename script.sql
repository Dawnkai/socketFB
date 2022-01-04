CREATE TABLE users (id INTEGER PRIMARY KEY, username TEXT, password TEXT);
CREATE TABLE friends (id INTEGER PRIMARY KEY, username TEXT, friend TEXT);
CREATE TABLE messages (id INTEGER PRIMARY KEY, sender TEXT, receiver TEXT, content TEXT);
