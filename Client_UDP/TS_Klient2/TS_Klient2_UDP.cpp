// TS_Klient_UDP.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
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
	SOCKET mainSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mainSocket == INVALID_SOCKET)
	{
		printf("Error creating socket: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Struktura sockaddr_in - reprezentuje adres IP
	sockaddr_in my_addr;
	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	my_addr.sin_port = htons(27016);

	sockaddr_in their_addr;
	memset(&their_addr, 0, sizeof(their_addr)); // Wyzeruj reszte struktury
	their_addr.sin_family = AF_INET; // host byte order
	their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	their_addr.sin_port = htons(27016); // short, network byte order

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

	bytesSent = sendto(mainSocket, sendbuf, strlen(sendbuf), 0, (SOCKADDR *)& their_addr, sizeof(struct sockaddr));
	//printf("Bytes sent: %ld\n", bytesSent);
	printf("Number sent: %s\n", sendbuf);

	int addr_len = sizeof(struct sockaddr);
	while (bytesRecv == SOCKET_ERROR)
	{
		bytesRecv = recvfrom(mainSocket, recvbuf, 64, 0, (SOCKADDR *)& their_addr, &addr_len);

		if (bytesRecv == 0)
		{
			printf("Connection closed.\n");
			closesocket(mainSocket);
			WSACleanup();
			break;
		}

		if (bytesRecv < 0)
			return 1;

		//printf("Bytes received: %ld\n", bytesRecv);
		printf("Received answer: \n%s", recvbuf);
	}
	printf("Connection closing...\n");
	closesocket(mainSocket); // Rozlaczenie
	WSACleanup();

	system("pause");
	return 0;
}

