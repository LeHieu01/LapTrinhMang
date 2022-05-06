#include <stdio.h>
#include <WinSock2.h>
#include <string.h> 
#include <stdlib.h> 
#include <string>
#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

#define INFO_BUFFER_SIZE 32767

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8000);

    system("pause");

    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        ret = WSAGetLastError();
        printf("Connect success - %d\n", ret);
        return 1;
    }

    int action = 1;

    char buf[256];

    char buffer[2048];

    while (true) {
        int helloFromServer = recv(client, buffer, sizeof(buffer), 0);

        if (helloFromServer <= 0)
        {
            if (helloFromServer == -1) {
                helloFromServer = WSAGetLastError();
            }
            break;
        }

        if (helloFromServer < sizeof(buffer)) {
            buffer[helloFromServer] = 0;
        }

        printf("%s \n", buffer);

        break;
    }

    while (action == 1)
    {

        printf("Write massage to sent to server\n");

        string input;

        getline(cin, input);

        // string message = "Information by client: " + input;

        const char* output = input.c_str();

        send(client, output, strlen(output), 0);

        int valread = recv(client, buf, sizeof(buf), 0);

        if (valread <= 0)
        {
            if (valread == -1) {
                valread = WSAGetLastError();
            }
            break;
        }

        if (valread < sizeof(buf)) {
            buf[valread] = 0;
        }

        printf("Massage to server: %s\n", buf);

        printf("---------------------------- \n \n");

        if (strcmp((char*)&input, "bye") == 0)
        {
            action = 0;
        }
        fflush(stdin);
    }

    closesocket(client);

    WSACleanup();
}