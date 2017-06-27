//
// Created by yingzi on 2017/6/26.
//

#include "SerialClient.h"
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::vector;

bool SerialClient::init() {
    if (!openPort()) {
        cerr << "open " << port << " failed!" << endl;
        return false;
    }
    cout << "open " << port << " success!" << endl;
    if (!setPort()) {
        cerr << "set " << port << " failed!" << endl;
        return false;
    }
    cout << "set " << port << "success!" << endl;
    return true;
}

void SerialClient::closePort() {
    close(fd);
}

bool SerialClient::setPort() {
    struct termios options;
    if (tcgetattr(fd, &options) != 0) {
        cerr << "SetupSerial failed!" << endl;
        return false;
    }

    vector<int> baudRates{38400, 19200, 9600, 4800, 2400, 1200, 300, 38400, 19200, 9600, 4800, 2400, 1200, 300};
    vector<int> baudRatesBin{B38400, B19200, B9600, B4800, B2400, B1200, B300, B38400, B19200, B9600, B4800, B2400, B1200, B300};

    for (auto it = baudRates.cbegin(), itb = baudRatesBin.cbegin(); it != baudRates.cend(); ++it, ++itb) {
        if (*it == baudRate) {
            cfsetispeed(&options, *itb);
            cfsetospeed(&options, *itb);
        }
    }

    options.c_cflag |= CLOCAL;
    options.c_cflag |= CREAD;
    switch(parityBit){
        case 0 :
            options.c_cflag &= ~CRTSCTS;
            break;
        case 1 :
            options.c_cflag |= CRTSCTS;
            break;
        case 2 :
            options.c_cflag |= IXON | IXOFF | IXANY;
            break;
    }

    options.c_cflag &= ~CSIZE;
    switch (dataBit){
        case 5 :
            options.c_cflag |= CS5;
            break;
        case 6	:
            options.c_cflag |= CS6;
            break;
        case 7	:
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag |= CS8;
            break;
        default:
            cerr << "Unsupported data size" << endl;
            return false;
    }

    switch (parity) {
        case 'n':
        case 'N':
            options.c_cflag &= ~PARENB;
            options.c_iflag &= ~INPCK;
            break;
        case 'o':
        case 'O':
            options.c_cflag |= (PARODD | PARENB);
            options.c_iflag |= INPCK;
            break;
        case 'e':
        case 'E':
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            options.c_iflag |= INPCK;
            break;
        case 's':
        case 'S':
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            break;
        default:
            cerr << "Unsupported parity" << endl;
            return false;
    }

    switch (stopBit){
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:
            options.c_cflag |= CSTOPB;
            break;
        default:
            cerr << "Unsupported stop bits" << endl;
            return false;
    }
    options.c_oflag &= ~OPOST;

    options.c_cc[VTIME] = 1;
    options.c_cc[VMIN] = 1;

    tcflush(fd,TCIFLUSH);

    if (tcsetattr(fd, TCSANOW, &options) != 0) {
        cerr << "com set error" << endl;
        return false;
    }
    return true;
}

bool SerialClient::openPort() {
    fd = open(port.c_str(), O_RDWR|O_NOCTTY|O_NDELAY);
    if (fd < 0){
        cerr << "Can't Open Serial Port" << port << endl;
        return false;
    }
    if(fcntl(fd, F_SETFL, 0) < 0){
        cerr << "fcntl failed!" << endl;
        return false;
    }
    if(0 == isatty(STDIN_FILENO)){
        cerr << port << " is not a terminal device" << endl;
        return false;
    }
    return true;
}

string SerialClient::recvMessage() {
    static const int maxBufSize = 1024;

    int buffer[maxBufSize] = {0};
    int len = read(fd, buffer, maxBufSize);
    return string(std::begin(buffer), std::begin(buffer) + len);
}

bool SerialClient::sendMessage(const string &message) {
    cout << message << endl;

    int len = write(fd, message.c_str(), message.size());
    if (len == message.size()) {
        cout << "success write " << len << " bytes" << endl;
        return true;
    }
    else {
        cerr << "write " << len << "/" << message.size() << " bytes" << endl;
        tcflush(fd, TCOFLUSH);
        return false;
    }
}

