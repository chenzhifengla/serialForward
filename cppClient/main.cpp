//
// Created by yingzi on 2017/6/27.
//

#include "SerialClient.h"
#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::ostringstream;

string getEventName() {
    static double p = 0.7;  // increase事件的概率
    if (rand() > RAND_MAX * p) return "decrease";
    else return "increase";
}

int getValueNum() {
    return rand() % 60;
}

int main(int argc, char** argv) {
    SerialClient serialClient("/dev/ttyS0");
    serialClient.init();

    while (1) {
        ostringstream oss;
        oss << "<event name=\"" << getEventName() << "\" value=\"x = " << getValueNum() << "\"/>";
        if (!serialClient.sendMessage(oss.str())) {
            cerr << "Send message " << oss.str() << "failed!" << endl;
            break;
        }
        sleep(1);
    }

    serialClient.closePort();

    return 0;
}