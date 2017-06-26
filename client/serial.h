//
// Created by root on 16-10-31.
//

#ifndef SERIALFORWARD_SERIAL_H
#define SERIALFORWARD_SERIAL_H

#endif //SERIALFORWARD_SERIAL_H

#define FALSE  -1
#define TRUE   0
#define MAXSIZE 1024
#define MAX_LINE_LEN 20
#define WAIT_SECOND 0
#define WAIT_USECOND 100000

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termios.h>
#include<errno.h>
#include<string.h>

int UART_Open(int fd,char* port);
void UART_Close(int fd);
int UART_Set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity);
int UART_Init(int fd, int speed,int flow_ctrlint ,int databits,int stopbits,char parity);
int UART_Recv(int fd, char *rcv_buf,int data_len);
int UART_Send(int fd, char *send_buf,int data_len);