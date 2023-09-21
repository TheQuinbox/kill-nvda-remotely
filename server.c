#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <ini.h>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPTSTR cmd_line, int cmd_show) {
	ini_t *config = ini_load("config.ini");
	if (config == NULL) {
		MessageBox(NULL, "Configuration file not found", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(1);
	}
	const char *port = ini_get(config, NULL, "port");
	if (!port) {
		MessageBox(NULL, "Invalid configuration file", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(1);
	}
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
		MessageBox(NULL, "WSAStartup failed", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(1);
	}
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET) {
		MessageBox(NULL, "Failed to create socket", "Error", MB_OK | MB_ICONERROR);
		WSACleanup();
		ExitProcess(1);
	}
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(port));
	server_address.sin_addr.s_addr = INADDR_ANY;
	if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
		MessageBox(NULL, "Bind failed", "Error", MB_OK | MB_ICONERROR);
		closesocket(server_socket);
		WSACleanup();
		ExitProcess(1);
	}
	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
		MessageBox(NULL, "Listen failed", "Error", MB_OK | MB_ICONERROR);
		closesocket(server_socket);
		WSACleanup();
		ExitProcess(1);
	}
	while (1) {
		SOCKET client_socket = accept(server_socket, NULL, NULL);
		if (client_socket == INVALID_SOCKET) {
			MessageBox(NULL, "Accept failed", "Error", MB_OK | MB_ICONERROR);
			closesocket(server_socket);
			WSACleanup();
			ExitProcess(1);
		}
		char buffer[1024];
		int bytes_received;
		while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
			buffer[bytes_received] = '\0';
			if (strcmp(buffer, "kill") == 0) {
				const char* command = "start nvda -r";
				system(command);
			}
		}
		closesocket(client_socket);
	}
	closesocket(server_socket);
	WSACleanup();
	ExitProcess(0);;
}
