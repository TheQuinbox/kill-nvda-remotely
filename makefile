CC = gcc
CFLAGS = -lws2_32 -s -static

CLIENT_SRC = client.c
SERVER_SRC = server.c
CLIENT_EXE = client.exe
SERVER_EXE = server.exe

all: $(CLIENT_EXE) $(SERVER_EXE)

$(CLIENT_EXE): $(CLIENT_SRC)
	$(CC) $(CLIENT_SRC) -o $(CLIENT_EXE) $(CFLAGS)

$(SERVER_EXE): $(SERVER_SRC)
	$(CC) $(SERVER_SRC) -o $(SERVER_EXE) $(CFLAGS)

clean:
	rm -f $(CLIENT_EXE) $(SERVER_EXE)

.PHONY: all clean
