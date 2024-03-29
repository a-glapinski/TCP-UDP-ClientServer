// TS_Serwer2.cpp : Defines the entry point for the console application.
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
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // SOCKET do nasluchiwania nowych polaczen
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Error creating socket: %ld\n", WSAGetLastError());
		WSACleanup(); 
		return 1;
	}

	// Struktura sockaddr_in - reprezentuje adres IP
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr)); // Wyzeruj reszte struktury
	addr.sin_family = AF_INET; // host byte order
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // lokalny host
	addr.sin_port = htons(27016); // short, network byte order

	// Przypisanie gniazda do adresu
	if (bind(listenSocket, (SOCKADDR *)& addr, sizeof(addr)) == SOCKET_ERROR)
	{
		printf("bind() failed.\n");
		closesocket(listenSocket);
		return 1;
	}

	// Nasluchiwanie - listen(deskryptor gniazda, backlog - ilosc dozwolonych polaczen w kolejce polaczen przychodzacych)
	if (listen(listenSocket, 1) == SOCKET_ERROR)
		printf("Error listening on socket.\n");

	SOCKET clientSocket = SOCKET_ERROR; // SOCKET do utrzymania polaczenia z klientem
	printf("Waiting for a client to connect...\n");

	// Przyjecie proby polaczenia od klienta, zwraca wskaznik na gniazdo, ktore bedzie sluzylo do komunikacji z klientem
	while (clientSocket == SOCKET_ERROR)
	{
		clientSocket = accept(listenSocket, NULL, NULL);
	}

	//// Jesli polaczenie zostalo nawiazane, zastepujemy uchwyt naszego gniazda tym otrzymanym z accept – tamto bylo potrzebne tylko do polaczenia sie z klientem
	//// listenSocket = clientSocket;
	printf("Client connected.\n");
	// Gniazdo nasluchujace nie jest dalej potrzebne
	closesocket(listenSocket);

	// Wysylanie i odbieranie danych
	int bytesSent;
	int bytesRecv = SOCKET_ERROR;
	char recvbuf[32] = "";

	bytesRecv = recv(clientSocket, recvbuf, 32, 0);
	//printf("Bytes received: %ld\n", bytesRecv);
	printf("Received number: %s\n", recvbuf);

	long long number = atoll(recvbuf);

	if (number <= 0 || !isPerfect(number)) {
		char sendbuf[64];
		std::string s = tostr(number, 0);
		strcpy_s(sendbuf, s.c_str());
		bytesSent = send(clientSocket, sendbuf, strlen(sendbuf), 0);
		printf("Answer sent: \n%s", sendbuf);
	}
	else {
		char sendbuf[64];
		std::string s = tostr(number, 1);
		strcpy_s(sendbuf, s.c_str());
		bytesSent = send(clientSocket, sendbuf, strlen(sendbuf), 0);
		printf("Answer sent: \n%s", sendbuf);
	}

	//printf("Bytes sent: %ld\n", bytesSent);

	printf("Connection closed.\n");
	WSACleanup();

	system("pause");
    return 0;
}

