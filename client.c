#include <winsock2.h>
#include <ini.h>

int main(int argc, const char *argv[]) {
	ini_t *config = ini_load("config.ini");
	if (config == NULL) {
		MessageBox(NULL, "Configuration file not found", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(1);
	}
	const char *host = ini_get(config, NULL, "host");
	const char *port = ini_get(config, NULL, "port");
	if (!host || !port) {
		MessageBox(NULL, "Invalid configuration file", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(1);
	}
	const char *message = "kill";
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
		MessageBox(NULL, "Failed to start the server", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(1);
	}
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET) {
		MessageBox(NULL, "Failed to create socket", "Error", MB_OK | MB_ICONERROR);
		WSACleanup();
		ExitProcess(1);
	}
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(port));
	server_address.sin_addr.s_addr = inet_addr(host);
	if (server_address.sin_addr.s_addr == INADDR_NONE) {
		MessageBox(NULL, "Invalid server address", "Error", MB_OK | MB_ICONERROR);
		closesocket(client_socket);
		WSACleanup();
		ExitProcess(1);
	}
	if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
		MessageBox(NULL, "Failed to connect to the server", "Error", MB_OK | MB_ICONERROR);
		closesocket(client_socket);
		WSACleanup();
		ExitProcess(1);
	}
	if (send(client_socket, message, strlen(message), 0) == SOCKET_ERROR) {
		MessageBox(NULL, "Failed to send data", "Error", MB_OK | MB_ICONERROR);
	}
	closesocket(client_socket);
	WSACleanup();
	ExitProcess(0);;
}
