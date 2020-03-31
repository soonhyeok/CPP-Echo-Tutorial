#define _WINSOCK_DEPRECATED_NO_WARNINGS
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
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876;
	char received[256];
	string sent;

	// winsock �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		showErrorMessage("WSAStartup()");
	}

	// TCP ���� ����
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (clientSocket == INVALID_SOCKET) {
		showErrorMessage("socket()");
	}

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	// ���ڿ� IP�� ��Ʈ��ũ ����Ʈ ��������
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	// 2����Ʈ ���� ��Ʈ��ũ ����Ʈ ��������
	serverAddress.sin_port = htons(serverPort);
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		showErrorMessage("connect()");
	}
	cout << "[�������] connect()\n";
	
	//�ݺ������� ������ �޽����� �����ϰ� ���ڸ޽����� ���� ����
	while(1) {
		cout << "[�޽��� ����]";
		getline(cin, sent);
		if (sent == "") {
			continue;
		}
		send(clientSocket, sent.c_str(), sent.length(), 0);
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0';
		if (strcmp(received, "[exit]") == 0) {
			cout << "[��������]\n";
			break;
		}
		cout << "[���� �޽���]: " << received << '\n';
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}