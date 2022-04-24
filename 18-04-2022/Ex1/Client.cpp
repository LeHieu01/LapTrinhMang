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
TCHAR  infoBuf[INFO_BUFFER_SIZE];
TCHAR  buf[INFO_BUFFER_SIZE];

DWORD  bufCharCount = INFO_BUFFER_SIZE;

DWORD disksCase, dwSectPerClust, dwBytesPerSect, dwFreeClusters, dwTotalClusters;

int main() {

    // Khoi tao thu vien
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);


    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8000);

    system("pause");

    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR) {
        ret = WSAGetLastError();
        printf("Ket noi khong thanh cong - %d\n", ret);
        return 1;
    }

    int action = 1;

    int i;

    LPCWSTR pszDrive = NULL;

    BOOL fResult;

    disksCase = GetLogicalDriveStrings(bufCharCount, (LPWSTR)buf);

    cout << ("1. Nhan 1 de gui thong tin ve ten may \n");
    cout << ("2. Nhan 2 de gui thong tin ve danh sach o dia \n");
    cout << ("3. Nhan 3 de gui thong tin ve kich thuoc o dia \n");
    cout << ("Nhan 0 de dung chuong trinh \n");

    int input;

    while (action == 1) {
        scanf("%d", &input);

        switch (input) {
        case 1:
            if (GetComputerName(infoBuf, &bufCharCount)) {

                wstring test(&infoBuf[0]); //convert to wstring

                string test2(test.begin(), test.end()); //and convert to string.              

                string message = "Machine's name is: " + test2;

                const char* lapName = message.c_str();

                send(client, lapName, strlen(lapName), 0);
            }
            break;
        case 2:
            if (disksCase != 0) {
                const char* disks = "";

                string message = "The logical drives of this machine are: ";

                // Check up to 100 drives...
                for (i = 0; i < 100; i++) {
                    wstring test(&buf[i]); //convert to wstring

                    string test2(test.begin(), test.end()); //and convert to string.

                    message += test2.c_str();

                    disks = message.c_str();
                }

                send(client, disks, strlen(disks), 0);
            }
            break;
        case 3:
            if (true) {
                fResult = GetDiskFreeSpace(pszDrive,

                    &dwSectPerClust,

                    &dwBytesPerSect,

                    &dwFreeClusters,

                    &dwTotalClusters);

                wstring wstr = to_wstring(dwTotalClusters);

                string res(wstr.begin(), wstr.end());

                string message = "Total cluster: " + res;

                const char* diskSpace = message.c_str();

                send(client, diskSpace, strlen(diskSpace), 0);
            }
            break;
        default:
            action = 0;
            break;
        }
    }

    closesocket(client);

    WSACleanup();
}