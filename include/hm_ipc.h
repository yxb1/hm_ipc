#ifndef _HM_IPC_H_
#define _HM_IPC_H_

#include <string>
#include <map>
#include <pthread.h>
extern "C"{
#include "hm_buffer.h"
} 
#include "tinyxml.h"

#define TOPIC_NAME_MAX_LEN 16
#define TOPIC_MAX_NUM 32

#pragma pack(1)
typedef struct
{
    char name[TOPIC_NAME_MAX_LEN];
    uint32_t total_len;
    HM_BUFF buff_info;
}SHT_HEADER;
#pragma pack()
/*-+----------+------------------+----------------+---------+-*
   |SHT_HEAHER|TOPIC_ARRARY_1    |TOPIC_ARRARY_2  | ....    |
 *-+----------+------------------+----------------+---------+-*/
#pragma pack(1)
typedef struct
{
    uint32_t used_num;
    char init_done;
    pthread_mutex_t _mutex;
    SHT_HEADER sht_head[TOPIC_MAX_NUM];
    uint32_t data_len;
    char sht_data[0];
}SHT_BUFF;
#pragma pack()

class HMIpc {
public:
    HMIpc() = default;
    ~HMIpc() = default;

    int32_t HMIpcInit();
    int32_t HMIpcWrite(std::string topic, void *src);
    char * HMIpcReadLatest(std::string topic);

private:
    SHT_BUFF *sht_bf;
    SHT_BUFF local;
};

#endif
