#ifndef __RTT_INTERFACE_H
#define __RTT_INTERFACE_H

extern rt_sem_t    key_sem;

extern rt_thread_t key_thread;
extern rt_thread_t led_thread;
extern rt_thread_t lcd_thread;
extern rt_thread_t dht11_thread;

void key_thread_entry(void* parameter);
void led_thread_entry(void* parameter);
void lcd_thread_entry(void* parameter);
void dht11_thread_entry(void* parameter);

#endif
