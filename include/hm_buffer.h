#ifndef _HM_BUFFER_H_
#define _HM_BUFFER_H_

#include <stdlib.h>
#include <stdio.h>

#pragma pack(1)
typedef struct {
    char *data;
    unsigned char front;
    unsigned char rear;
    unsigned int dt_len;
    unsigned int size;
}HM_BUFF;
#pragma pack()

void hm_buff_init(HM_BUFF *buff, char *addr);
char hm_buff_push(HM_BUFF *buff, void *dst, void *src);
char * hm_buff_pop_latest(HM_BUFF *buff, void *src);

#endif
