#include <stdio.h>

#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_WARNINGS
DWORD WINAPI ClientThread(LPVOID);
void RemoveClient(SOCKET);

SOCKET clients[64];
int numClients;

DWORD WINAPI ClientThread(LPVOID lpParam)
{
    SOCKET client = *(SOCKET*)lpParam;
    char buf[256];
    char buf2[10] = "Enter id\n";
    char id[40];
    int ret;
    snprintf(id, sizeof(id), "client_id:%d", client);
    printf(id);
    while (1) {
        send(client, buf2, strlen(buf2), 0);
        ret = recv(client, buf, sizeof(buf), 0);
        buf[ret] = 0;
        if (strcmp(buf, id) == 0) {
            send(client, "da ket noi\n", 11, 0);
            break;
        }
    }
    while (1)
    {

        // Nhan du lieu tu client
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0) {
            break;
        }
        buf[ret] = 0;
        printf("Received: %s\n", buf);


        char cha[256];
        snprintf(cha, sizeof(cha), "%d: %s", client, buf);
        for (int i = 0;i < numClients; i++) {
            if (clients[i] != client)
                send(clients[i], cha, sizeof(cha), 0);
        }
        closesocket(client);
    }
}
int main()
    {
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);

        SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        SOCKADDR_IN addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(9000);

        bind(listener, (SOCKADDR*)&addr, sizeof(addr));
        listen(listener, 5);

        numClients = 0;

        while (1)
        {

            SOCKET client = accept(listener, NULL, NULL);
            // Them client vao mang
            clients[numClients] = client;
            numClients++;
            printf("New client accepted: %d\n", client);
            CreateThread(0, 0, ClientThread, &client, 0, 0);
        }
    }
