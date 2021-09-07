#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void showLabel(string label) {
	cout << "[" << label << "]";
}

void showFormatMessage(string label, string message) {
	showLabel(label);
	cout << ": " << message;
}

void showErrorMessage(string message) {
	showFormatMessage("���� �߻�", message);
	cout << endl;
	system("pause");
	exit(1);
}

void showCurrentMessage(string message) {
	showFormatMessage("���� ����", message);
	cout << endl;
}

int main(void) {
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876;
	char received[256];
	string sent;

	// 1. winsock �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		showErrorMessage("WSAStartup()");

	// 2. ���� ����
	clientSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP ���� ����

	if (clientSocket == INVALID_SOCKET)
		showErrorMessage("socket()");

	// ���� �ּ� ����ü �ʱ�ȭ
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; // �ּ� ü��: IPv4
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // ���ڿ� IP�� ��Ʈ��ũ ����Ʈ �������� ��ȯ�ؼ� ���� �ּҸ� �־��ش�.
	serverAddress.sin_port = htons(serverPort); // 2����Ʈ ������ ��Ʈ��ũ ����Ʈ �������� ��ȯ�ؼ� ��Ʈ ��ȣ�� �־��ش�.

	// 3. ������ �����ϱ�
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		showErrorMessage("connect()");

	showCurrentMessage("connect()");

	// �ݺ������� ������ �޼����� �����ϰ� ���� �޼����� ���� �ޱ�
	while (1) {
		showFormatMessage("�޼��� ����", "");
		getline(cin, sent);

		if (sent == "") continue;

		// 4. �������� �ַܼ� ���� �޼��� ����
		send(clientSocket, sent.c_str(), sent.length(), 0);

		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;

		if (strcmp(received, "[exit]") == 0) {
			showLabel("���� ����");
			cout << endl;
			break;
		}

		showFormatMessage("���� �޼���", string(received));
		cout << endl;
	}

	// 8. ��� ���� �� Ŭ�� ������ �ݾ��ְ� winSock api�� ��ȯ�ϱ�
	closesocket(clientSocket);
	WSACleanup();

	system("pause");
	return 0;
}