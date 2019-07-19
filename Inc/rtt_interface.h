#ifndef __RTT_INTERFACE_H
#define __RTT_INTERFACE_H

extern rt_sem_t    key_sem;
extern rt_sem_t    mode_sem;

extern rt_thread_t key_thread;
extern rt_thread_t manual_mode_thread;
extern rt_thread_t lcd_thread;
extern rt_thread_t dht11_thread;
extern rt_thread_t bh1750_thread;
extern rt_thread_t auto_mode_thread;
extern rt_thread_t mode_thread;

void key_thread_entry(void* parameter);
void manual_mode_thread_entry(void* parameter);
void lcd_thread_entry(void* parameter);
void dht11_thread_entry(void* parameter);
void bh1750_thread_entry(void* parameter);
void auto_mode_thread_entry(void* parameter);
void mode_thread_entry(void* parameter);

#endif
