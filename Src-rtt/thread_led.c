#include <entry.h>
#include "stm32f4xx_hal.h"

/* PD12 --- LED_Green
 * PD13 --- LED_Orange
 * PD14 --- LED_Red
 * PD15 --- LED_Blue
 */

extern rt_sem_t key_sem;

/* key_val = 0, none   key pressed
 * key_val = 1, right  key pressed
 * key_val = 2, up     key pressed
 * key_val = 3, down   key pressed
 * key_val = 4, left   key pressed
 * key_val = 5, select key pressed
 */
uint8_t led_toggle_done = 0;

rt_thread_t led_thread     = RT_NULL;
rt_thread_t key_rcv_thread = RT_NULL;

void led_thread_entry(void* parameter)
{
  while(1) {
    rt_sem_take(key_sem, RT_WAITING_FOREVER);
    
    if(led_toggle_done==0) {
      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
      led_toggle_done = 1;
    }
    rt_thread_delay(10);
  }
}
