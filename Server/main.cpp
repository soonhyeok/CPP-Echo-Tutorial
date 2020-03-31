#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void showErrorMessage(string message) {
	cout << "[오류발생]: " << message << '\n';
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256];

	// winsock 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		showErrorMessage("WSASTARTUP()");
	}
	// TCP 소켓 초기화
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (serverSocket == INVALID_SOCKET) {
		showErrorMessage("socket()");
	}

	// 서버 어드레스 0으로 초기화
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; // IPV4 사용
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // 4바이트 정수 네트워크 바이트 형식으로 
	serverAddress.sin_port = htons(serverPort); // 2바이트 정수 네트워크바이트 형식으로

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		showErrorMessage("bind()");
	}
	cout << "[현재 상태] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) {
		showErrorMessage("Listen()");
	}
	cout << "[현재 상태] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[현재 상태] accept()\n";
	if (clientSocket == INVALID_SOCKET) {
		showErrorMessage("accetpt()");
	}

	//클라이언트 메시지를 받아 그대로 다시 전달
	while(1) {
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		cout << "[클라이언트 메시지]: " << received << '\n';
		cout << "[메시지 전송]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[서버 종료]\n";
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0);
	}

	closesocket(clientSocket);
	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}