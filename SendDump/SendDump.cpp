// SendDump.cpp : Defines the entry point for the console application.
// The corresponding server.c is in the https://github.com/Junch/socket/transfer

#include "stdafx.h"
#include <winsock2.h>
#include <Ws2tcpip.h>
#include "guard.h"

#define    MAXLINE        1024

#pragma comment(lib, "Ws2_32.lib")

void usage(char *command)
{
    printf("usage :%s ipaddr portnum filename\n", command);
    exit(0);
}

int writen(SOCKET fd, const void *vptr, size_t n)
{
    size_t  nleft;
    int     nwritten;
    const char  *ptr = (const char *)vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nwritten = send(fd, ptr, nleft, NULL)) == SOCKET_ERROR) {
            return SOCKET_ERROR;
        }

        nleft -= nwritten;
        ptr += nwritten;
    }

    return n;
}

int sendFileName(SOCKET fd, char* filename){
    int flen = strlen(filename);
    int flenSend = htonl(flen);
    writen(fd, &flenSend, 4);
    return writen(fd, filename, flen);
}

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    auto& r = RAII::make_ScopeGuard([](){ //auto& r cannot be removed
        WSACleanup();
    });

    if (argc != 4) {
        usage(argv[0]);
    }

    SOCKET sock_id;
	if ((sock_id = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
        perror("Create socket failed\n");
        exit(0);
    }

    auto& r2 = RAII::make_ScopeGuard([&](){
        closesocket(sock_id);
    });

    struct sockaddr_in  serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    //inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

    int i_ret = connect(sock_id, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
    if (-1 == i_ret) {
        printf("Connect socket failed\n");
        return -1;
    }

    char path_buffer[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath(argv[3], drive, dir, fname, ext); // C4996 
    _makepath(path_buffer, NULL, NULL, fname, ext);
    //following is used in linux
    //char *fname = basename(argv[3]);
    if (sendFileName(sock_id, path_buffer) < 0){
        perror("Send file name failed\n");
        return -1;
    }


    // If it is "r", it will be read as text. The fread may return before the EOF.
    FILE *fp = NULL;
    if ((fp = fopen(argv[3], "rb")) == NULL) {
        perror("Open file failed\n");
        exit(0);
    }

    char buf[MAXLINE];
    memset(buf, 0, MAXLINE);
    int read_len, send_len;
    while ((read_len = fread(buf, sizeof(char), MAXLINE, fp)) >0) {
        send_len = writen(sock_id, buf, read_len);
        if (send_len < 0) {
            perror("Send file failed\n");
            fclose(fp);
            exit(0);
        }
        memset(buf, 0, MAXLINE);
    }

    fclose(fp);

    printf("Send Finish\n");

	return 0;
}
