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
	showFormatMessage("오류 발생", message);
	cout << endl;
	system("pause");
	exit(1);
}

void showCurrentMessage(string message) {
	showFormatMessage("현재 상태", message);
	cout << endl;
}

int main(void) {
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876;
	char received[256];
	string sent;

	// 1. winsock 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		showErrorMessage("WSAStartup()");

	// 2. 소켓 생성
	clientSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓 생성

	if (clientSocket == INVALID_SOCKET)
		showErrorMessage("socket()");

	// 서버 주소 구조체 초기화
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; // 주소 체계: IPv4
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // 문자열 IP를 네트워크 바이트 형식으로 변환해서 서버 주소를 넣어준다.
	serverAddress.sin_port = htons(serverPort); // 2바이트 정수를 네트워크 바이트 형식으로 변환해서 포트 번호를 넣어준다.

	// 3. 서버와 연결하기
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		showErrorMessage("connect()");

	showCurrentMessage("connect()");

	// 반복적으로 서버로 메세지를 전송하고 에코 메세지를 전달 받기
	while (1) {
		showFormatMessage("메세지 전송", "");
		getline(cin, sent);

		if (sent == "") continue;

		// 4. 서버에게 콘솔로 받은 메세지 전송
		send(clientSocket, sent.c_str(), sent.length(), 0);

		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;

		if (strcmp(received, "[exit]") == 0) {
			showLabel("서버 종료");
			cout << endl;
			break;
		}

		showFormatMessage("서버 메세지", string(received));
		cout << endl;
	}

	// 8. 통신 종료 후 클라 소켓을 닫아주고 winSock api도 반환하기
	closesocket(clientSocket);
	WSACleanup();

	system("pause");
	return 0;
}