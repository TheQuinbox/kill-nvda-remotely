#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ini.h>

int main(int argc, const char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <server> <port>\n", argv[0]);
		return 1;
	}
	const char *server = argv[1];
	const char *port = argv[2];
	const char *message = "kill";
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
		fprintf(stderr, "WSAStartup failed\n");
		return 1;
	}
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET) {
		fprintf(stderr, "Failed to create socket\n");
		WSACleanup();
		return 1;
	}
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(port));
	server_address.sin_addr.s_addr = inet_addr(server);
	if (server_address.sin_addr.s_addr == INADDR_NONE) {
		fprintf(stderr, "Invalid server address\n");
		closesocket(client_socket);
		WSACleanup();
		return 1;
	}
	if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
		fprintf(stderr, "Failed to connect to the server\n");
		closesocket(client_socket);
		WSACleanup();
		return 1;
	}
	if (send(client_socket, message, strlen(message), 0) == SOCKET_ERROR) {
		fprintf(stderr, "Failed to send data\n");
	}
	closesocket(client_socket);
	WSACleanup();
	return 0;
}
