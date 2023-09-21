#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

int main(int argc, const char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		return 1;
	}
	const char* port = argv[1];
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
		fprintf(stderr, "WSAStartup failed\n");
		return 1;
	}
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET) {
		fprintf(stderr, "Failed to create socket\n");
		WSACleanup();
		return 1;
	}
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(port));
	server_address.sin_addr.s_addr = INADDR_ANY;
	if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
		fprintf(stderr, "Bind failed\n");
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}
	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
		fprintf(stderr, "Listen failed\n");
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}
	printf("Server listening on port %s\n", port);
	while (1) {
		SOCKET client_socket = accept(server_socket, NULL, NULL);
		if (client_socket == INVALID_SOCKET) {
			fprintf(stderr, "Accept failed\n");
			closesocket(server_socket);
			WSACleanup();
			return 1;
		}
		char buffer[1024];
		int bytes_received;
		while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
			buffer[bytes_received] = '\0';
			if (strcmp(buffer, "kill") == 0) {
				const char* command = "start nvda -r";
				if (system(command) == 0) {
					printf("Restarted NVDA\n");
				} else {
					printf("Couldn't restart NVDA!\n");
				}
			}
		}
		closesocket(client_socket);
	}
	closesocket(server_socket);
	WSACleanup();
	return 0;
}
