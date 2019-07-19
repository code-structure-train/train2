#include <entry.h>
#include "stm32f4xx_hal.h"

extern float    light_lx;
extern rt_sem_t key_sem;
extern uint8_t  current_mode;

rt_thread_t auto_mode_thread = RT_NULL;

void auto_mode_thread_entry(void* parameter)
{
  while(1) {    
    if(current_mode == 1){
      if(light_lx < 200.0f) {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
      } else {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
      }
    }
    rt_thread_delay(10);
  }
}
