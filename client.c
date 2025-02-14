#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
	char* pszHost = {0};
	char* pszPort = {0};
	const char* pszConfigFile = "config.ini";
	if (GetPrivateProfileString("Settings", "Host", NULL, pszHost, sizeof(pszHost), pszConfigFile) == 0 || GetPrivateProfileString("Settings", "Port", NULL, pszPort, sizeof(pszPort), pszConfigFile) == 0) {
		MessageBox(NULL, "Invalid or missing configuration file", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(1);
	}
	const char* pszMessage = "kill";
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		MessageBox(NULL, "Failed to initialize Winsock", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(1);
	}
	SOCKET hClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (hClientSocket == INVALID_SOCKET) {
		MessageBox(NULL, "Failed to create socket", "Error", MB_OK | MB_ICONERROR);
		WSACleanup();
		ExitProcess(1);
	}
	struct sockaddr_in stServerAddress = {0};
	stServerAddress.sin_family = AF_INET;
	stServerAddress.sin_port = htons((u_short)atoi(pszPort));
	stServerAddress.sin_addr.s_addr = inet_addr(pszHost);
	if (stServerAddress.sin_addr.s_addr == INADDR_NONE) {
		MessageBox(NULL, "Invalid server address", "Error", MB_OK | MB_ICONERROR);
		closesocket(hClientSocket);
		WSACleanup();
		ExitProcess(1);
	}
	if (connect(hClientSocket, (struct sockaddr*)&stServerAddress, sizeof(stServerAddress)) == SOCKET_ERROR) {
		MessageBox(NULL, "Failed to connect to the server", "Error", MB_OK | MB_ICONERROR);
		closesocket(hClientSocket);
		WSACleanup();
		ExitProcess(1);
	}
	if (send(hClientSocket, pszMessage, strlen(pszMessage), 0) == SOCKET_ERROR) {
		MessageBox(NULL, "Failed to send data", "Error", MB_OK | MB_ICONERROR);
	}
	closesocket(hClientSocket);
	WSACleanup();
	ExitProcess(0);
}
