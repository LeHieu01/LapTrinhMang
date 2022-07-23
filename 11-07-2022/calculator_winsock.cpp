// HTTPServer1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
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
    char buf[2048];
    int ret;
    while (1)
    {
        // Chap nhan ket noi
        SOCKET client = accept(listener, NULL, NULL);
        // Nhan yeu cau
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
        {
            closesocket(client);
            continue;
        }
        // Xu ly yeu cau
        if (ret < sizeof(buf))
            buf[ret] = 0;
        printf("%s\n", buf);
        if (strncmp(p1, "GET /calc?op=", 13) == 0)
        {
            //Get operator
            char* p2 = strstr(p1, "?op=");
            char* p5 = strstr(p1, "&");
            int opLength = p5 - p2 - 4;
            char* op = (char*)malloc(opLength + 1), * param1, * param2;
            memcpy(op, p2 + 4, opLength);
            op[opLength] = 0;
            //printf("%s\n", op);
            //check parameters condition
            char* p3 = strstr(p1, "param1=");
            char* p4 = strstr(p1, "param2=");
            if (p3 == NULL || p4 == NULL) {
                const char* res = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Thieu tham so</h1></body></html>";
                send(client, res, strlen(res), 0);
            }
            else {
                if ((p3 - p4) < 0) {
                    //Get parameter 1
                    int param1length = p4 - p3 - 1 - 7;
                    param1 = (char*)malloc(param1length + 1);
                    memcpy(param1, p3 + 7, param1length);
                    param1[param1length] = 0;
                    //Get parameter 2
                    p5 = strstr(p4, " ");
                    int param2length = p5 - p4 - 7;
                    param2 = (char*)malloc(param2length + 1);
                    memcpy(param2, p4 + 7, param2length);
                    param2[param2length] = 0;
                    //printf(" %s %s \n", param1, param2);

                }
                else {
                    //Get parameter 2
                    int param2length = p3 - p4 - 1 - 7;
                    param2 = (char*)malloc(param2length + 1);
                    memcpy(param2, p4 + 7, param2length);
                    param2[param2length] = 0;
                    //Get parameter 1
                    p5 = strstr(p3, " ");
                    int param1length = p5 - p3 - 7;
                    param1 = (char*)malloc(param1length + 1);
                    memcpy(param1, p3 + 7, param1length);
                    param1[param1length] = 0;
                    //printf(" %s %s \n", param1, param2);
                }
                //Print result on website
                int a = atoi(param1);
                int b = atoi(param2);
                if (strcmp(op, "add") == 0) {

                    sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Du tham so</h1><h2>%d+%d=%d</h2></body></html>", a, b, (a + b));
                    send(client, buf, strlen(buf), 0);
                }
                else if (strcmp(op, "sub") == 0) {

                    sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Du tham so</h1><h2>%d-%d=%d</h2></body></html>", a, b, (a - b));
                    send(client, buf, strlen(buf), 0);
                }
                else if (strcmp(op, "mul") == 0) {

                    sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Du tham so</h1><h2>%d*%d=%d</h2></body></html>", a, b, (a * b));
                    send(client, buf, strlen(buf), 0);
                }
                else if (strcmp(op, "div") == 0) {

                    sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Du tham so</h1><h2>%d/%d=%f</h2></body></html>", a, b, ((float)a / b));
                    send(client, buf, strlen(buf), 0);
                }
                else {
                    const char* res = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Phep toan sai</h1></body></html>";
                    send(client, res, strlen(res), 0);
                }

            }
        }
        else
        {
            // Tra lai ket qua
            const char* res = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello World</h1></body></html>";
            send(client, res, strlen(res), 0);
        }

        // Dong ket noi
        closesocket(client);

    }
    WSACleanup();
}
