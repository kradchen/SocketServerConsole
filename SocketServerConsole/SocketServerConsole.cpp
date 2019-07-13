// SocketServerConsole.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "winsock2.h"
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512
const char * DEFAULT_PORT = "8888";
int main()
{
	WSAData wsaData;
	int startResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	if (startResult != 0)
	{
		printf("WAS start up failed! Error code:%d", startResult);
		return 0;
	}
	printf("WSAStartup!\r\n");
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	int iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	printf("getaddrinfo success!\r\n");
	SOCKET sock = INVALID_SOCKET;
	sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	printf("socket created!\r\n");
	// Setup the TCP listening socket
	iResult = bind(sock, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	printf("socket bind to port: %s!\r\n", DEFAULT_PORT);
	freeaddrinfo(result);
	if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	printf("start listen on port: %s!\r\n", DEFAULT_PORT);
	SOCKET ClientSocket = INVALID_SOCKET;

	// Accept a client socket
	ClientSocket = accept(sock, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	printf("socket accpted!\r\n");
	char recvbuf[DEFAULT_BUFLEN];
	int iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Receive until the peer shuts down the connection
	do {
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

	} while (iResult > 0);
	scanf("%d",iResult);
	if (WSACleanup() == SOCKET_ERROR)
	{
		printf("WAS clean up failed! Error code:%d", WSAGetLastError());	
		WSACleanup();
		return -1;
	}
    return 0;
}

