#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <memory.h>
#include <unistd.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C"{
#endif
#include "hm_buffer.h"
#ifdef __cplusplus 
} 
#endif 
#include "hm_ipc.h"
#include "parse_xml.h"

using namespace std;

int main(int argc, const char *argv[]) {
    std::shared_ptr<HMIpc> _ipc = std::make_shared<HMIpc>();
    _ipc->HMIpcInit();
    char test[50000] = {0};
    uint8_t tick = 'a';
    while(true) {
        tick++;
        if(tick == 'z') tick = 'a';
        memset(test, tick, 50000);
        usleep(10*1000);
        _ipc->HMIpcWrite("camera_eq4_data", test);
        _ipc->HMIpcWrite("vehinfo_data", test);
    }
}