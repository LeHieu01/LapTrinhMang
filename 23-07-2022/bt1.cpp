#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(110);
    connect(client, (SOCKADDR*)&addr, sizeof(addr));
    

    char sendcmd[256];
    char buf[2048];

    int ret = recv(client, buf, sizeof(buf), 0);
    buf[ret] = 0;
    printf("%s", buf);
    
   
    
    // Dang nhap
    while (1)
    {
        char user[32], pass[32];
        printf("Nhap username: ");
        scanf("%s", user);
        // Gui lenh USER
        sprintf(sendcmd, "USER %s\r\n", user);
        send(client, sendcmd, strlen(sendcmd), 0);
        ret = recv(client, buf, sizeof(buf), 0);
        buf[ret] = 0;

        // Gui lenh PASS
        printf("Nhap password: ");
        scanf("%s", pass);
  
        sprintf(sendcmd, "PASS %s\r\n", pass);
        send(client, sendcmd, strlen(sendcmd), 0);
        ret = recv(client, buf, sizeof(buf), 0);
        buf[ret] = 0;
        //printf("%s", buf);
        // Xu ly ket qua lenh PASS
        if (strncmp(buf, "+OK", 3) == 0)
        {
            printf("Dang nhap thanh cong!\n");
            break;
        }
        else
        {
            printf("Dang nhap that bai. Hay nhap lai.\n");
        }
    }
    // Gui lenh LIST
    strcpy(sendcmd, "LIST\r\n");
    send(client, sendcmd, strlen(sendcmd), 0);
    ret = recv(client, buf, sizeof(buf), 0);
    buf[ret] = 0;
    printf("%s", buf);

    while (1)
    {
    }
    closesocket(client);
    WSACleanup();
}
