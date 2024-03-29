// TS_Klient2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <winsock2.h>
#include <string>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

//#define L 33550336l

int main()
{
	// Inicjacja Winsock
	WSADATA wsaData;

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
		printf("Initialization error.\n");

	// Utworzenie deskryptora gniazda - socket(AF_INET - rodzina adresow IPv4, typ gniazda, protokol)
	SOCKET connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connectSocket == INVALID_SOCKET)
	{
		printf("Error creating socket: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Struktura sockaddr_in - reprezentuje adres IP
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(27016);

	// Aby klient polaczyl sie z serwerem pod wybranym wczesniej adresem, musimy wywolac funkcje connect. Podajemy jej gniazdo oraz strukture sockaddr_in z wlasciwym adresem serwera
	if (connect(connectSocket, (SOCKADDR *)& addr, sizeof(addr)) == SOCKET_ERROR)
	{
		printf("Failed to connect.\n");
		WSACleanup();
		return 1;
	}
	printf("Connected!\n");

	// Wysylanie i odbieranie przez klienta
	int num;
	std::cout << "Enter number: \n";
	std::cin >> num;

	int bytesSent;
	int bytesRecv = SOCKET_ERROR;
	std::string s = std::to_string(num);
	char sendbuf[32];
	strcpy_s(sendbuf, s.c_str());
	char recvbuf[64] = "";

	bytesSent = send(connectSocket, sendbuf, strlen(sendbuf), 0);
	//printf("Bytes sent: %ld\n", bytesSent);
	printf("Number sent: %s\n", sendbuf);

	while (bytesRecv == SOCKET_ERROR)
	{
		bytesRecv = recv(connectSocket, recvbuf, 64, 0);

		if (bytesRecv == 0 || bytesRecv == WSAECONNRESET)
		{
			printf("Connection closed.\n");
			closesocket(connectSocket);
			WSACleanup();
			break;
		}

		if (bytesRecv < 0)
			return 1;

		//printf("Bytes received: %ld\n", bytesRecv);
		printf("Received answer: \n%s", recvbuf);
	}
	printf("Connection closing...\n");
	closesocket(connectSocket); // Rozlaczenie
	WSACleanup();
	system("pause");
    return 0;
}

