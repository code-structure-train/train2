#ifndef __RTT_INTERFACE_H
#define __RTT_INTERFACE_H

extern rt_mq_t     key_mq;

extern rt_thread_t key_thread;
extern rt_thread_t led_thread;
extern rt_thread_t key_send_thread;
extern rt_thread_t key_rcv_thread;

void key_thread_entry(void* parameter);
void led_thread_entry(void* parameter);
void key_send_entry(void* parameter);
void key_receive_entry(void* parameter);

#endif
