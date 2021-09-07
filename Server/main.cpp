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
	showFormatMessage("오류 발생", message);
	system("pause");
	exit(1);
}

void showCurrentMessage(string message) {
	showFormatMessage("현재 상태", message);
}

int main(void) {
	WSADATA wsaData; // winsock 구조체
	SOCKET serverSocket, clientSocket; // 서버, 클라 소켓 구조체
	SOCKADDR_IN serverAddress, clientAddress; // 서버, 클라 소켓 주소 구조체

	int serverPort = 9876;
	char received[256];

	// 1. winsock 초기화 
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Winsock 초기화 (2.2 버전)
		showErrorMessage("WSAStartup()");

	// 2. 소켓 생성 (socket)
	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓 생성 (주소 체계: IPv4, 소켓 타입: TCP 타입)

	if (serverSocket == INVALID_SOCKET) // 서버 소켓 생성 에러 처리
		showErrorMessage("socket()");

	// 서버 주소 구조체 초기화
	memset(&serverAddress, 0, sizeof(serverAddress)); // 서버 주소를 0으로 초기화
	serverAddress.sin_family = AF_INET; // 주소 체계: IPv4 (PF_INET == AF_INET)
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // 4바이트 정수를 네트워크 바이트 형식으로 변환해서 서버 주소를 넣어준다.
	serverAddress.sin_port = htons(serverPort); // 2바이트 정수를 네트워크 바이트 형식으로 변환해서 포트 번호를 넣어준다.

	// 3. 소켓과 네트워크 인터페이스를 묶기 (bind)
	// IPv4 소켓 주소 구조체를 일반 소켓 구조체(SOCKADDR)로 형변환
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		showErrorMessage("bind()");

	showCurrentMessage("bind()");

	// 4. 클라 요청 기다리기 (동시 연결 클라이언트 최대 5개로 설정)
	if (listen(serverSocket, 5) == SOCKET_ERROR)
		showErrorMessage("listen()");

	showCurrentMessage("listen()");

	// 5. 클라 요청이 들어왔을 때 연결 수락하기
	// accept 되는 순간 비어있던 clientAddress 구조체에 클라 서버 정보가 채워지고
	// 클라와 통신할 수 있는 소켓을 반환한다.
	int clientAddressSize = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &clientAddressSize);

	if (clientSocket == INVALID_SOCKET)
		showErrorMessage("accept()");

	showCurrentMessage("accept()");

	// 클라이언트의 메세지를 받아서 그대로 다시 전달
	while (1) {
		// 6. 클라 소켓으로부터 받은 메세지를 received 문자열 버퍼에 저장
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL; // 문자열의 마지막은 NULL을 줘서 예기치 못한 문자를 잘라냄.

		showFormatMessage("클라이언트 메세지", string(received));
		showFormatMessage("메세지 전송", string(received));

		// 7. 클라 소켓에게 받은 메세지를 전송
		// sizeof(received) - 1 의미는 마지막 enter 칠 때의 개행 문자는 제외한다는 의미
		send(clientSocket, received, sizeof(received) - 1, 0);

		// 통신 종료
		if (strcmp(received, "[exit]") == 0) {
			showLabel("서버 종료");
			cout << endl;
			break;
		}
	}

	// 8. 통신 종료 후 서버 소켓과 클라 소켓을 닫아주고 winSock api도 반환하기
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	
	system("pause");
	return 0;
}