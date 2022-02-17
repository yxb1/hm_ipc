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
#include "hm_buffer.h"
#include "hm_ipc.h"
#include "parse_xml.h"

using namespace std;

int main(int argc, const char *argv[]) {
    std::shared_ptr<HMIpc> _ipc = std::make_shared<HMIpc>();
    _ipc->HMIpcInit();

    char *test = NULL;
    while(true) {
        usleep(50*1000);
        test = _ipc->HMIpcReadLatest("camera_eq4_data");
#if 0
        if(test != NULL) {
            test[50000] = 0;
            std::cout << "camera_eq4_data: " << test << std::endl;
        }
#endif
        test = _ipc->HMIpcReadLatest("vehinfo_data");
#if 0
        if(test != NULL) {
            test[50000] = 0;
            std::cout << "vehinfo_data:    " << test << std::endl;
        }
#endif
    }
}