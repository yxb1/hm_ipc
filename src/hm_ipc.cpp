#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <mutex>
#include <pthread.h>
#include "hm_ipc.h"
#include "parse_xml.h"
#include "hm_buffer.h"

int32_t HMIpc::HMIpcInit() {
    SHT_BUFF tmp;
    std::string path = "../xml/topic_info.xml";
    std::shared_ptr<ParseXML> ptr = std::make_shared<ParseXML>();
    ptr->xml_parser(path.c_str(), &tmp);
    std::mutex init_mutex;
    {
        std::lock_guard<std::mutex> locker(init_mutex);
        //key_t key = ftok("sht", 0x456);
        key_t key = 0x123456;
        int shmid = shmget(key, (sizeof(SHT_BUFF)+tmp.data_len), (0666|IPC_CREAT));
        std::cout << "key: " << key << "; shmid: " << shmid << "; size: " << (sizeof(SHT_BUFF)+tmp.data_len) << std::endl;
        if(shmid == -1) {
            std::cout << "Create share memory failed." << strerror(errno) << std::endl;
            exit(-1);
        }

        void *sht_addr = NULL;
        sht_addr = shmat(shmid, 0, 0);
        if(sht_addr == NULL) {
            std::cout << "shmat failed." << strerror(errno) << std::endl;
            exit(-1);
        }
        sht_bf = (SHT_BUFF *)sht_addr;
#if 0
        if(!first) {
            uint32_t step = 0;
            for(int i = 0; i < sht_bf->used_num; i++) {
                local.sht_head[i].buff_info.data = ((char *)sht_bf + sizeof(SHT_BUFF) + step);
                step += sht_bf->sht_head[i].total_len;

                std::cout << "----------------------------------" << std::endl;
                std::cout << "[---" << i << "---]" << "sht_bf->used_num: " << (uint32_t)sht_bf->used_num << std::endl;
                std::cout << "[---" << i << "---]" << "sht_bf->data_len: " << sht_bf->data_len << std::endl;
                std::cout << "[---" << i << "---]" << "name: " << sht_bf->sht_head[i].name << std::endl;
                std::cout << "[---" << i << "---]" << "total_len: " << sht_bf->sht_head[i].total_len << std::endl;
                std::cout << "[---" << i << "---]" << "dt_len: " << sht_bf->sht_head[i].buff_info.dt_len << std::endl;
                std::cout << "[---" << i << "---]" << "size: " << sht_bf->sht_head[i].buff_info.size << std::endl;
                std::cout << "[---" << i << "---]" << "local addr: " << static_cast<void *>(local.sht_head[i].buff_info.data) << std::endl;
                std::cout << "[---" << i << "---]" << "data: " << static_cast<void *>(sht_bf->sht_head[i].buff_info.data) << std::endl;
            }
            return 1;
        }
#endif
        memcpy(sht_bf, &tmp, sizeof(SHT_BUFF));
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&sht_bf->_mutex, &attr);
    }

    uint32_t step = 0;
    for(int i = 0; i < sht_bf->used_num; i++) {
        local.sht_head[i].buff_info.data = ((char *)sht_bf + sizeof(SHT_BUFF) + step);
        hm_buff_init(&(sht_bf->sht_head[i].buff_info), (sht_bf->sht_data + step));
        step += sht_bf->sht_head[i].total_len;

        std::cout << "----------------------------------" << std::endl;
        std::cout << "[---" << i << "---]" << "sht_bf->used_num: " << sht_bf->used_num << std::endl;
        std::cout << "[---" << i << "---]" << "sht_bf->data_len: " << sht_bf->data_len << std::endl;
        std::cout << "[---" << i << "---]" << "name: " << sht_bf->sht_head[i].name << std::endl;
        std::cout << "[---" << i << "---]" << "total_len: " << sht_bf->sht_head[i].total_len << std::endl;
        std::cout << "[---" << i << "---]" << "dt_len: " << sht_bf->sht_head[i].buff_info.dt_len << std::endl;
        std::cout << "[---" << i << "---]" << "size: " << sht_bf->sht_head[i].buff_info.size << std::endl;
        std::cout << "[---" << i << "---]" << "local addr: " << static_cast<void *>(local.sht_head[i].buff_info.data) << std::endl;
        std::cout << "[---" << i << "---]" << "data: " << static_cast<void *>(sht_bf->sht_head[i].buff_info.data) << std::endl;
    }
}

int32_t HMIpc::HMIpcWrite(std::string topic, void *src) {
    for(int i = 0; i < sht_bf->used_num; i++) {
        if(strcmp(sht_bf->sht_head[i].name, topic.c_str()) == 0) {
            //std::cout << "Found " << sht_bf->sht_head[i].name << " when write" << std::endl;
            pthread_mutex_lock(&sht_bf->_mutex);
            hm_buff_push(&sht_bf->sht_head[i].buff_info, (void *)local.sht_head[i].buff_info.data, src);
            pthread_mutex_unlock(&sht_bf->_mutex);
        }
    }
}

char * HMIpc::HMIpcReadLatest(std::string topic) {
    for(int i = 0; i < sht_bf->used_num; i++) {
        if(strcmp(sht_bf->sht_head[i].name, topic.c_str()) == 0) {
            //std::cout << "Found " << sht_bf->sht_head[i].name << " when read" << std::endl;
            pthread_mutex_lock(&sht_bf->_mutex);
            char *ptr = hm_buff_pop_latest(&sht_bf->sht_head[i].buff_info, (void *)local.sht_head[i].buff_info.data);
            pthread_mutex_unlock(&sht_bf->_mutex);
            return ptr;
        }
    }
}
