#include <winsock2.h>
#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, const char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
		return 1;
	}
	const char* port = argv[1];
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
		std::cerr << "WSAStartup failed" << std::endl;
		return 1;
	}
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET) {
		std::cerr << "Failed to create socket" << std::endl;
		WSACleanup();
		return 1;
	}
	sockaddr_in server_address{};
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(std::stoi(port));
	server_address.sin_addr.s_addr = INADDR_ANY;
	if (bind(server_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) == SOCKET_ERROR) {
		std::cerr << "Bind failed" << std::endl;
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}
	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "Listen failed" << std::endl;
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}
	std::cout << "Server listening on port " << port << std::endl;
	while (true) {
		SOCKET client_socket = accept(server_socket, nullptr, nullptr);
		if (client_socket == INVALID_SOCKET) {
			std::cerr << "Accept failed" << std::endl;
			closesocket(server_socket);
			WSACleanup();
			return 1;
		}
		char buffer[1024];
		int bytes_received;
		while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
			buffer[bytes_received] = '\0';
			if (strcmp(buffer, "kill") == 0) {
				const char *command = "start nvda -r";
				if (system(command) == 0) {
					std::cout << "Restarted NVDA" << std::endl;
				} else {
					std::cout << "Couldn't restart NVDA!" << std::endl;
				}
			}
		}
		closesocket(client_socket);
	}
	closesocket(server_socket);
	WSACleanup();
	return 0;
}
