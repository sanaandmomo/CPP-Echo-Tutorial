#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void showLabel(string label) {
	cout << "[" << label << "]";
}

void showFormatMessage(string label, string message) {
	showLabel(label);
	cout << ": " << message << endl;
}

void showErrorMessage(string message) {
	showFormatMessage("���� �߻�", message);
	system("pause");
	exit(1);
}

void showCurrentMessage(string message) {
	showFormatMessage("���� ����", message);
}

int main(void) {
	WSADATA wsaData; // winsock ����ü
	SOCKET serverSocket, clientSocket; // ����, Ŭ�� ���� ����ü
	SOCKADDR_IN serverAddress, clientAddress; // ����, Ŭ�� ���� �ּ� ����ü

	int serverPort = 9876;
	char received[256];

	// 1. winsock �ʱ�ȭ 
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Winsock �ʱ�ȭ (2.2 ����)
		showErrorMessage("WSAStartup()");

	// 2. ���� ���� (socket)
	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP ���� ���� (�ּ� ü��: IPv4, ���� Ÿ��: TCP Ÿ��)

	if (serverSocket == INVALID_SOCKET) // ���� ���� ���� ���� ó��
		showErrorMessage("socket()");

	// ���� �ּ� ����ü �ʱ�ȭ
	memset(&serverAddress, 0, sizeof(serverAddress)); // ���� �ּҸ� 0���� �ʱ�ȭ
	serverAddress.sin_family = AF_INET; // �ּ� ü��: IPv4 (PF_INET == AF_INET)
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // 4����Ʈ ������ ��Ʈ��ũ ����Ʈ �������� ��ȯ�ؼ� ���� �ּҸ� �־��ش�.
	serverAddress.sin_port = htons(serverPort); // 2����Ʈ ������ ��Ʈ��ũ ����Ʈ �������� ��ȯ�ؼ� ��Ʈ ��ȣ�� �־��ش�.

	// 3. ���ϰ� ��Ʈ��ũ �������̽��� ���� (bind)
	// IPv4 ���� �ּ� ����ü�� �Ϲ� ���� ����ü(SOCKADDR)�� ����ȯ
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		showErrorMessage("bind()");

	showCurrentMessage("bind()");

	// 4. Ŭ�� ��û ��ٸ��� (���� ���� Ŭ���̾�Ʈ �ִ� 5���� ����)
	if (listen(serverSocket, 5) == SOCKET_ERROR)
		showErrorMessage("listen()");

	showCurrentMessage("listen()");

	// 5. Ŭ�� ��û�� ������ �� ���� �����ϱ�
	// accept �Ǵ� ���� ����ִ� clientAddress ����ü�� Ŭ�� ���� ������ ä������
	// Ŭ��� ����� �� �ִ� ������ ��ȯ�Ѵ�.
	int clientAddressSize = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &clientAddressSize);

	if (clientSocket == INVALID_SOCKET)
		showErrorMessage("accept()");

	showCurrentMessage("accept()");

	// Ŭ���̾�Ʈ�� �޼����� �޾Ƽ� �״�� �ٽ� ����
	while (1) {
		// 6. Ŭ�� �������κ��� ���� �޼����� received ���ڿ� ���ۿ� ����
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL; // ���ڿ��� �������� NULL�� �༭ ����ġ ���� ���ڸ� �߶�.

		showFormatMessage("Ŭ���̾�Ʈ �޼���", string(received));
		showFormatMessage("�޼��� ����", string(received));

		// 7. Ŭ�� ���Ͽ��� ���� �޼����� ����
		// sizeof(received) - 1 �ǹ̴� ������ enter ĥ ���� ���� ���ڴ� �����Ѵٴ� �ǹ�
		send(clientSocket, received, sizeof(received) - 1, 0);

		// ��� ����
		if (strcmp(received, "[exit]") == 0) {
			showLabel("���� ����");
			cout << endl;
			break;
		}
	}

	// 8. ��� ���� �� ���� ���ϰ� Ŭ�� ������ �ݾ��ְ� winSock api�� ��ȯ�ϱ�
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	
	system("pause");
	return 0;
}