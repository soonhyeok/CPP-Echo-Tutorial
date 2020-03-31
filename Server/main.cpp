#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void showErrorMessage(string message) {
	cout << "[�����߻�]: " << message << '\n';
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256];

	// winsock �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		showErrorMessage("WSASTARTUP()");
	}
	// TCP ���� �ʱ�ȭ
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (serverSocket == INVALID_SOCKET) {
		showErrorMessage("socket()");
	}

	// ���� ��巹�� 0���� �ʱ�ȭ
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; // IPV4 ���
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // 4����Ʈ ���� ��Ʈ��ũ ����Ʈ �������� 
	serverAddress.sin_port = htons(serverPort); // 2����Ʈ ���� ��Ʈ��ũ����Ʈ ��������

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		showErrorMessage("bind()");
	}
	cout << "[���� ����] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) {
		showErrorMessage("Listen()");
	}
	cout << "[���� ����] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[���� ����] accept()\n";
	if (clientSocket == INVALID_SOCKET) {
		showErrorMessage("accetpt()");
	}

	//Ŭ���̾�Ʈ �޽����� �޾� �״�� �ٽ� ����
	while(1) {
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		cout << "[Ŭ���̾�Ʈ �޽���]: " << received << '\n';
		cout << "[�޽��� ����]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[���� ����]\n";
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