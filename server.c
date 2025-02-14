#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
	char szPort[16] = {0};
	const char* szConfigFile = "config.ini";
	if (GetPrivateProfileString("Settings", "Port", NULL, szPort, sizeof(szPort), szConfigFile) == 0) {
		MessageBox(NULL, "Invalid or missing configuration file", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(1);
	}
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		MessageBox(NULL, "WSAStartup failed", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(1);
	}
	SOCKET hServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (hServerSocket == INVALID_SOCKET) {
		MessageBox(NULL, "Failed to create socket", "Error", MB_OK | MB_ICONERROR);
		WSACleanup();
		ExitProcess(1);
	}
	struct sockaddr_in stServerAddress = {0};
	stServerAddress.sin_family = AF_INET;
	stServerAddress.sin_port = htons(atoi(szPort));
	stServerAddress.sin_addr.s_addr = INADDR_ANY;
	if (bind(hServerSocket, (struct sockaddr*)&stServerAddress, sizeof(stServerAddress)) == SOCKET_ERROR) {
		MessageBox(NULL, "Bind failed", "Error", MB_OK | MB_ICONERROR);
		closesocket(hServerSocket);
		WSACleanup();
		ExitProcess(1);
	}
	if (listen(hServerSocket, SOMAXCONN) == SOCKET_ERROR) {
		MessageBox(NULL, "Listen failed", "Error", MB_OK | MB_ICONERROR);
		closesocket(hServerSocket);
		WSACleanup();
		ExitProcess(1);
	}
	while (1) {
		SOCKET hClientSocket = accept(hServerSocket, NULL, NULL);
		if (hClientSocket == INVALID_SOCKET) {
			MessageBox(NULL, "Accept failed", "Error", MB_OK | MB_ICONERROR);
			closesocket(hServerSocket);
			WSACleanup();
			ExitProcess(1);
		}
		char szBuffer[1024];
		int nBytesReceived;
		while ((nBytesReceived = recv(hClientSocket, szBuffer, sizeof(szBuffer) - 1, 0)) > 0) {
			szBuffer[nBytesReceived] = '\0';
			if (strcmp(szBuffer, "kill") == 0) system("start nvda -r");
		}
		closesocket(hClientSocket);
	}
	closesocket(hServerSocket);
	WSACleanup();
	ExitProcess(0);
}
