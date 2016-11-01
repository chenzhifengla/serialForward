//
// Created by root on 16-10-31.
//

#ifndef SERIALFORWARD_PTY_H
#define SERIALFORWARD_PTY_H

#endif //SERIALFORWARD_PTY_H

#include "pubHeader.h"

int PTY_Open(int fd, char* port);
void PTY_Close(int fd);
int PTY_Recv(int fd, char* rcv_buf, int data_len);
int PTY_Send(int fd, char* send_buf, int data_len);