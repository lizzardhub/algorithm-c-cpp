#include "stdafx.h"

#include <Windows.h>
#include <winsock.h>
#include <stdio.h>
#pragma comment(lib, "wsock32.lib")

#define SERVER 0
#define CLIENT 1
const int SIDE = CLIENT;
const int MAX_CONN = 100;

int remote_send(char *ip, int port, char *buffer, int len) {
	SOCKADDR_IN remote;
	remote.sin_family = AF_INET;
	remote.sin_port = htons(port);
	remote.sin_addr.s_addr = inet_addr(ip);
	SOCKET portal = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create socket
	if (portal == INVALID_SOCKET) {
		printf("Socket failed to initialize\n");
		return 1;
	}

	int retcode = connect(portal, (SOCKADDR *)&remote, sizeof(remote)); // Connect
	if (retcode != 0) {
		printf("Connect failed with code %d\n", retcode);
		return 1;
	}
	retcode = send(portal, buffer, len, 0); // Send
	printf("Sent %d bytes\n", retcode);

	closesocket(portal);
	return 0;
}

int remote_listen(int port, char *buffer, int len) {
	SOCKADDR_IN local, remote;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	SOCKET portal_local = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create socket
	if (portal_local == INVALID_SOCKET) {
		printf("Socket failed to initialize\n");
		return 1;
	}

	int retcode = bind(portal_local, (LPSOCKADDR)&local, sizeof(local)); // Bind
	if (retcode == SOCKET_ERROR) {
		printf("Socket failed to bind\n");
		closesocket(portal_local);
		return 1;
	}
	listen(portal_local, MAX_CONN);

	int c = sizeof(SOCKADDR_IN);
	printf("Accept\n");
	SOCKET portal = accept(portal_local, (SOCKADDR *)&remote, &c); // Accept
	if (portal == INVALID_SOCKET) {
		printf("Accept failed\n");
		closesocket(portal_local);
		return 1;
	}

	int recv_len = 0;
	while (recv_len < len) {
		printf("Recv\n");
		retcode = recv(portal, buffer + recv_len, len - recv_len, 0);
		if (retcode == -1) {
			printf("Recv failed\n");
			closesocket(portal);
			closesocket(portal_local);
			return 1;
		}
		printf("Received %d bytes\n", retcode);
		recv_len += retcode;
	}

	closesocket(portal_local);
	return 0;
}

void* allocate(size_t size) {
	void *p = malloc(size);
	if (p == NULL) {
		abort();
	}
	else {
		return p;
	}
}

void load(void) {
	WSADATA wsadata;
	int retcode = WSAStartup(MAKEWORD(1, 1), &wsadata);
	if (retcode != 0) printf("Winsock initialization failed with code %d\n", retcode);

	char *ip = "192.168.1.62";
	int port = 1543;
	if (SIDE == CLIENT) {
		FILE *inf = fopen("input", "rb");
		fseek(inf, (long int)0, SEEK_END);
		int size = ftell(inf);
		printf("File size: %d\n", size);
		fflush(stdout);
		fseek(inf, (long int)0, SEEK_SET);
		char *h = (char *)allocate(size);
		fread((void *)(h), sizeof(char), size, inf);

		long int transfer_size = htonl(size);
		remote_send(ip, port, (char *)&transfer_size, sizeof(transfer_size));
		Sleep(1000);
		remote_send(ip, port, h, size);

		fclose(inf);
	}
	else {
		long int transfer_size;
		remote_listen(port, (char *)&transfer_size, sizeof(transfer_size));
		transfer_size = ntohl(transfer_size);

		printf("Receiving file of %d bytes\n", transfer_size);
		char *h = (char *)allocate(transfer_size);
		remote_listen(port, h, transfer_size);

		FILE *ouf = fopen("output", "wb");
		fwrite(h, sizeof(char), transfer_size, ouf);
		fclose(ouf);
	}

	WSACleanup();
}

int main(void) {
	LARGE_INTEGER a, b, speed;
	QueryPerformanceFrequency(&speed);
	QueryPerformanceCounter(&a);

	load();
	/*FILE *ouf = fopen("output", "wb");
	char *h = (char *)allocate(1000);
	for (int i = 0; i < 1000; i++) h[i] = 'a' + i % ('z' - 'a');
	for (int i = 0; i < 100000; i++) {
		fwrite(h, sizeof(char), 1000, ouf);
	}
	fclose(ouf);*/

	QueryPerformanceCounter(&b);
	printf("Ticks elapsed: %lld. Tps: %lld.\n", b.QuadPart - a.QuadPart, speed.QuadPart);
	system("pause");


	return 0;
}
