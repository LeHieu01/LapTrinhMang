#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
SOCKET clients[64];
int numClients = 0;
map<int, string> listUser;
void RemoveClient(SOCKET client)
{
    // Tim vi tri cua client trong mang
    int i = 0;
    for (; i < numClients; i++)
        if (clients[i] == client) break;
    // Xoa client khoi mang
    if (i < numClients - 1)
        clients[i] = clients[numClients - 1];
    numClients--;


}
DWORD WINAPI ClientThread(LPVOID lpParam)
{
    SOCKET client = *(SOCKET*)lpParam;
    int ret;
    char buf[256];
    char user[32], pass[32], tmp[32];
    string mark;


    // Xu ly dang nhap
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            return 0;
        buf[ret] = 0;
        printf("Du lieu nhan duoc: %s\n", buf);
        ret = sscanf(buf, "%s %s %s", user, pass, tmp);
        if (ret != 2)
        {
            const char* msg = "Sai cu phap. Hay nhap lai.\n";
            send(client, msg, strlen(msg), 0);
        }
        else
        {
            FILE* f = fopen("D:\\Test.txt", "r");

            int found = 0;
            while (!feof(f))
            {
                char line[256];
                fgets(line, sizeof(line), f);

                char markLine[256];
                fgets(markLine, sizeof(markLine), f);

                char userf[32], passf[32], markf[10];
                sscanf(line, "%s %s", userf, passf);
                if (strcmp(user, userf) == 0 && strcmp(pass, passf) == 0)
                {
                    found = 1;

                    mark = sscanf(markLine, "%s", markf);

                    break;
                }
            }
            fclose(f);
            if (found == 1)
            {
                string msg_str = "Ok " + mark;
                const char* msg = msg_str.c_str();
                send(client, msg, strlen(msg), 0);
                //Gui thong bao co client ket noi moi
                const char* newMsg = "client moi ket noi\n";
                char newBuf[256];
                sprintf(newBuf, "%s: %s", user, newMsg);
                for (int i = 0; i < numClients; i++) {
                    send(clients[i], newBuf, strlen(newBuf), 0);
                }
                //Them vao mang
                clients[numClients] = client;

                listUser.insert({ client, user });

                numClients++;
                break;
            }
            else
            {
                const char* msg = "Khong tim thay tai khoan. Hay nhap lai.\n";
                send(client, msg, strlen(msg), 0);
            }
        }
    }


    closesocket(client);
    WSACleanup();
}
int main()
{
    // Khoi tao thu vien
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    // Tao socket
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Khai bao dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);
    // Gan cau truc dia chi voi socket
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    // Chuyen sang trang thai cho ket noi
    listen(listener, 5);
    while (1)
    {
        SOCKET client = accept(listener, NULL, NULL);
        printf("Client moi ket noi: %d\n", client);
        CreateThread(0, 0, ClientThread, &client, 0, 0);
    }
}
