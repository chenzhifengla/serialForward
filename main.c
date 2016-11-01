//
// Created by root on 16-10-31.
//

#include "serial.h"
#include "pty.h"
#include <signal.h>
#define MAXSIZE 512
int fd_serial = FALSE, fd_pty = FALSE;

void quit(int signum){
    //printf("catch signal SIGINT\n");
    UART_Close(fd_serial);
    //printf("\nclose serial\n");
    PTY_Close(fd_pty);
    exit(0);
}

int main(){
    char* serialName = "/dev/ttyS0";
    char* ptyName = "/dev/pts/0";

    int ret = FALSE;
    char buf[MAXSIZE];
    memset(buf, 0, sizeof(buf));

    //serial open and init
    fd_serial = UART_Open(fd_serial, serialName);
    if(FALSE == fd_serial){
        printf("open \"%s\" error\n", serialName);
        return 1;
    }
    ret = UART_Init(fd_serial,9600,0,8,1,'N');
    if (FALSE == ret){
        printf("Set Port \"%s\" Error\n", serialName);
        return 1;
    }

    //pty open and init
    fd_pty = PTY_Open(fd_pty, ptyName);
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
            printf("\"%s\" ---->> \"%s\" : %s\n", serialName, ptyName, buf);
            PTY_Send(fd_pty, buf, ret);
        }
        else {
            //printf("\"%s\" -->> \"%s\"\n", serialName, ptyName);
        }

        ret = PTY_Recv(fd_pty, buf, MAXSIZE);
        if (ret > 0){
            buf[ret] = '\0';
            printf("\"%s\" <<---- \"%s\" : %s\n", serialName, ptyName, buf);
            UART_Send(fd_serial, buf, ret);
        }
        else{
            //printf("\"%s\" --> \"%s\"\n", ptyName, serialName);
        }
    }

    return 0;
}

