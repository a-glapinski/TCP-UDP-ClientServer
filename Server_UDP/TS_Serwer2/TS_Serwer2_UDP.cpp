// TS_Serwer_UDP.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "stdafx.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <winsock2.h>
#include <string>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")

bool isPerfect(long long number)
{
	long long sum = 0;
	for (long long i = 1; i <= number / 2; i++)
	{
		if (number % i == 0)
		{
			sum += i;
		}
	}
	if (sum == number)
		return true;
	else
		return false;
}

//bool isPerfect(long long number) {
//	// nie istnieje znana liczba doskonala nieparzysta
//	if (number % 2 == 1) {
//		return false;
//	}
//
//	long long result = 1; // dzielnik 1
//	long long i = 2;
//	while (i * i <= number) { // dopoki i <= sqrt(number)
//		if (number % i == 0) {
//			result += i;
//			result += number / i;
//		}
//		i++;
//	}
//	if (i * i == number) { // i*i, sqrt(number) dodane dwa razy
//		result -= i;
//	}
//	return result == number;
//}

std::string tostr(long long x, bool isPerfect)
{
	std::stringstream ss;
	if (!isPerfect) {
		ss << x << " is not a perfect number.\n";
		return ss.str();
	}
	else {
		ss << x << " is a perfect number.\n";
		return ss.str();
	}
}

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
	memset(&my_addr, 0, sizeof(my_addr)); // Wyzeruj reszte struktury
	my_addr.sin_family = AF_INET; // host byte order
	my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	my_addr.sin_port = htons(27016); // short, network byte order

	sockaddr_in their_addr;
	memset(&their_addr, 0, sizeof(their_addr)); // Wyzeruj reszte struktury
	their_addr.sin_family = AF_INET; // host byte order
	their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	their_addr.sin_port = htons(27016); // short, network byte order

	// Przypisanie gniazda do adresu
	if (bind(mainSocket, (SOCKADDR *)& my_addr, sizeof(my_addr)) == SOCKET_ERROR)
	{
		printf("bind() failed.\n");
		closesocket(mainSocket);
		return 1;
	}

	printf("Waiting for a client...\n");

	// Wysylanie i odbieranie danych
	int bytesSent;
	int bytesRecv = SOCKET_ERROR;
	char recvbuf[32] = "";

	int addr_len = sizeof(struct sockaddr);
	bytesRecv = recvfrom(mainSocket, recvbuf, 32, 0, (SOCKADDR *)& their_addr, &addr_len);
	//printf("Bytes received: %ld\n", bytesRecv);
	printf("Received number: %s\n", recvbuf);

	long long number = atoll(recvbuf);

	if (number <= 0 || !isPerfect(number)) {
		char sendbuf[64];
		std::string s = tostr(number, 0);
		strcpy_s(sendbuf, s.c_str());
		bytesSent = sendto(mainSocket, sendbuf, strlen(sendbuf), 0, (SOCKADDR *)& their_addr, sizeof(struct sockaddr));
		printf("Answer sent: \n%s", sendbuf);
	}
	else {
		char sendbuf[64];
		std::string s = tostr(number, 0);
		strcpy_s(sendbuf, s.c_str());
		bytesSent = sendto(mainSocket, sendbuf, strlen(sendbuf), 0, (SOCKADDR *)& their_addr, sizeof(struct sockaddr));
		printf("Answer sent: \n%s", sendbuf);
	}

	//printf("Bytes sent: %ld\n", bytesSent);

	printf("Connection closed.\n");
	WSACleanup();

	system("pause");
	return 0;
}