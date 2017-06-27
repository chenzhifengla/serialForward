//
// Created by yingzi on 2017/6/26.
//

#ifndef RUNTIME_VERIFICATION_SYSTEM_SERIALFORWARD_H
#define RUNTIME_VERIFICATION_SYSTEM_SERIALFORWARD_H

#include <string>
using std::string;

class SerialClient {
  public:
    explicit SerialClient(const string& name):port(name) {}
    bool init();  // 设置串口、打开串口
    void closePort();

    string recvMessage();    // 收消息
    bool sendMessage(const string& message); // 发消息

  private:
    int fd = -1; // 串口的文件描述符
    string port;    // 串口名称

    static const int baudRate = 9600;    // 波特率
    static const int parityBit = 0;      // 校验位
    static const int dataBit = 8;    // 数据位
    static const int stopBit = 1;   // 停止位
    static const char parity = 'N';     // 奇偶校验位

    bool openPort();    // 打开串口
    bool setPort();  // 设置串口，设置波特率、校验位等
};


#endif //RUNTIME_VERIFICATION_SYSTEM_SERIALFORWARD_H
