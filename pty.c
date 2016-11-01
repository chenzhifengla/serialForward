//
// Created by root on 16-10-31.
//

#include "pty.h"

int PTY_Open(int fd, char* port){
    fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (FALSE == fd){
        perror("Can't Open Serial Port");
        return (FALSE);
    }

    if (fcntl(fd, F_SETFL, 0) < 0){
        printf("fcntl failed!\n");
        return (FALSE);
    }
    if (0 == isatty(STDIN_FILENO)){
        printf("\"%s\" is not a terminal device\n", port);
        return (FALSE);
    }
    return fd;
}

void PTY_Close(int fd){
    close(fd);
}

int PTY_Recv(int fd, char* rcv_buf, int data_len){
    int len, fs_sel;
    fd_set fs_read;
    struct timeval time;
    FD_ZERO(&fs_read);
    FD_SET(fd, &fs_read);

    time.tv_sec = 0;
    time.tv_usec = 0;

    fs_sel = select(fd + 1, &fs_read, NULL, NULL, &time);
    if (fs_sel){
        len = read(fd, rcv_buf, data_len);
        return len;
    } else{
        return (FALSE);
    }
}

int PTY_Send(int fd, char *send_buf,int data_len)
{
    int ret;

    ret = write(fd,send_buf,data_len);
    if (data_len == ret ){
        return ret;
    } else {
        tcflush(fd,TCOFLUSH);
        return FALSE;

    }

}
