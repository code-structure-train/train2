#include <entry.h>
#include "stm32f4xx_hal.h"
#include "LCD.h"
#include "bh1750.h"

extern rt_sem_t sensor_sem;

float light_lx;

rt_thread_t bh1750_thread = RT_NULL;

void bh1750_thread_entry(void* parameter)
{
  rt_err_t uwRet = RT_EOK;
  float BH1750_lx;
  while(1) {
    rt_sem_take(sensor_sem, RT_WAITING_FOREVER);  
    
    if(BH1750_OK == BH1750_ReadLight(&BH1750_lx)) {
      light_lx = BH1750_lx;
    }
    rt_thread_delay(10);
    
    uwRet =  rt_sem_release(sensor_sem);
    if(RT_EOK != uwRet) {
      rt_kprintf("sensor sem release error, code: %lx\n", uwRet);
    }
  }
}
