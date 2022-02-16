#include <string.h>
#include "hm_buffer.h"

void hm_buff_init(HM_BUFF *buff, char *addr)
{
    buff->data = addr;
    buff->front = 0;
    buff->rear = 0;
}

char hm_buff_empty(HM_BUFF *buff)
{
    return (buff->front == buff->rear)?1:0;
}

char hm_buff_full(HM_BUFF *buff)
{
    return ((buff->rear+1)%buff->size == buff->front)?1:0;
}

void hm_buff_clear(HM_BUFF *buff)
{
    buff->data = NULL;
    buff->front = 0;
    buff->rear = 0;
    buff->dt_len = 0;
    buff->size = 0;
}

char hm_buff_push(HM_BUFF *buff, void *dst, void *src)
{
    if(src == NULL) return -1;
    if(hm_buff_full(buff))
    {
        //skip oldest data
        buff->front = (buff->front + 1)%buff->size;
    }
    unsigned int index = (buff->dt_len*buff->rear);
    //printf("%p %p %d\n", ((char *)dst + index), src, buff->dt_len);
    memcpy(((char *)dst + index), src, buff->dt_len);
    buff->rear = (buff->rear + 1)%buff->size;
    return 0;
}

char * hm_buff_pop_latest(HM_BUFF *buff, void *src)
{
    if(hm_buff_empty(buff)) return NULL;
    unsigned int index = 0;
    if(buff->rear == 0)
    {
        index = (buff->size - 1)*buff->dt_len;
    }
    else
    {
        index = (buff->rear - 1)*buff->dt_len;
    }
    //printf("ggggggggggg %p %d\n", ((char *)src + index), buff->dt_len);
    //memcpy((char *)dst, ((char *)src + index), buff->dt_len);
    //char *ptr = (char *)dst;
    return ((char *)src + index);
    /*Considering the one to many situation, the following code will not be retained*/
    //hm_buff->front = (hm_buff->front + 1)%hm_buff->size;
}
