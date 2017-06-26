//
// Created by root on 16-10-31.
//

#include "serial.h"
#include <signal.h>
#include <string.h>

int fd_serial = FALSE, fd_pty = FALSE;
char serialName[20] = "/dev/ttyS0";
char ptyName[20] = "/dev/pts/1";

void quit(int signum){
    printf("catch signal SIGINT\n");
    UART_Close(fd_serial);
    printf("关闭 \"%s\"\n", serialName);
    UART_Close(fd_pty);
    printf("关闭 \"%s\"\n", ptyName);
    exit(0);
}

int main(int argc, char** argv){

    if (argc == 3) {
        strcpy(serialName, argv[1]);
        strcpy(ptyName, argv[2]);
    } else if (argc == 2) {
        strcpy(serialName, argv[1]);
    } else if (argc != 1) {
        printf("Usage: ./serialForward serialName ptyName\n");
        return 1;
    }

    int ret = FALSE;
    char buf[MAXSIZE];
    memset(buf, 0, sizeof(buf));

    //serial open and init
    fd_serial = UART_Open(fd_serial, serialName);
    if(FALSE == fd_serial){
        printf("open \"%s\" error\n", serialName);
        return 1;
    }
    else {
        printf("打开 \"%s\"\n", serialName);
    }
    ret = UART_Init(fd_serial,9600,0,8,1,'N');
    if (FALSE == ret){
        printf("Set Port \"%s\" Error\n", serialName);
        return 1;
    }
    else{
        printf("打开 \"%s\"\n", ptyName);
    }

    //pty open and init
    fd_pty = UART_Open(fd_pty, ptyName);
    if (FALSE == fd_pty){
        printf("open \"%s\" error\n", ptyName);
        return 1;
    }

    signal(SIGINT, quit);

    //loop
    while(1){
        ret = UART_Recv(fd_serial, buf, MAXSIZE);
        if (ret > 0){
            buf[ret] = '\0';
//            printf("\"%s\" ---->> \"%s\" : %s\n", serialName, ptyName, buf);
            printf("外部设备 ---->> 虚拟机 : %s\n", buf);
            UART_Send(fd_pty, buf, ret);
        }
        else {
//            printf("\"%s\" -->> \"%s\"\n", serialName, ptyName);
        }

        ret = UART_Recv(fd_pty, buf, MAXSIZE);
        if (ret > 0){
            buf[ret] = '\0';
//            printf("\"%s\" <<---- \"%s\" : %s\n", serialName, ptyName, buf);
            printf("外部设备 <<---- 虚拟机 : %s\n", buf);
            UART_Send(fd_serial, buf, ret);
        }
        else{
//            printf("\"%s\" --> \"%s\"\n", ptyName, serialName);
        }
    }

    return 0;
}

