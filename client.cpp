#include <winsock2.h>
#include <iostream>
#include <string>

int main(int argc, const char *argv[]) {
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <server> <port>" << std::endl;
		return 1;
	}
	const char *server = argv[1];
	const char *port = argv[2];
	const char *message = "kill";
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
		std::cerr << "WSAStartup failed" << std::endl;
		return 1;
	}
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET) {
		std::cerr << "Failed to create socket" << std::endl;
		WSACleanup();
		return 1;
	}
	sockaddr_in server_address{};
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(std::stoi(port));
	server_address.sin_addr.s_addr = inet_addr(server);
	if (server_address.sin_addr.s_addr == INADDR_NONE) {
		std::cerr << "Invalid server address" << std::endl;
		closesocket(client_socket);
		WSACleanup();
		return 1;
	}
	if (connect(client_socket, reinterpret_cast<sockaddr *>(&server_address), sizeof(server_address)) == SOCKET_ERROR) {
		std::cerr << "Failed to connect to the server" << std::endl;
		closesocket(client_socket);
		WSACleanup();
		return 1;
	}
	if (send(client_socket, message, strlen(message), 0) == SOCKET_ERROR) {
		std::cerr << "Failed to send data" << std::endl;
	}
	closesocket(client_socket);
	WSACleanup();
	return 0;
}
