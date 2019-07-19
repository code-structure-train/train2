#include <entry.h>
#include "stm32f4xx_hal.h"

extern rt_sem_t    mode_sem;
extern rt_thread_t manual_mode_thread;
extern rt_thread_t auto_mode_thread;
extern rt_thread_t lcd_thread;

/* current_mode = 0, manual mode
* current_mode = 1, auto mode
*/
uint8_t current_mode = 0;

/* key_val = 0, none   key pressed
 * key_val = 1, right  key pressed
 * key_val = 2, up     key pressed
 * key_val = 3, down   key pressed
 * key_val = 4, left   key pressed
 * key_val = 5, select key pressed
*/
uint8_t mode_switch_done = 0;

rt_thread_t mode_thread = RT_NULL;

void mode_thread_entry(void* parameter)
{
  
  while(1) {
    rt_sem_take(mode_sem, RT_WAITING_FOREVER);
    
    if(mode_switch_done == 0) {
      switch(current_mode) {
      case 0: {
        current_mode = 1;
        break;
      }
      case 1: {
        current_mode = 0;
        break;
      }
      }
      mode_switch_done = 1;
      rt_schedule_insert_thread(lcd_thread);
    }
    rt_thread_delay(10);
  }
}
