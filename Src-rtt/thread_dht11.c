#include <entry.h>
#include "stm32f4xx_hal.h"
#include "dht11.h"

extern rt_sem_t sensor_sem;

rt_thread_t dht11_thread = RT_NULL;

DHT11_Dev dht11;

void dht11_thread_entry(void* parameter)
{
  rt_err_t uwRet = RT_EOK;
  while(1) {  
    rt_sem_take(sensor_sem, RT_WAITING_FOREVER);  
    
    DHT11_Read(&dht11);
    rt_thread_delay(10);
    
    uwRet =  rt_sem_release(sensor_sem);
    if(RT_EOK != uwRet) {
      rt_kprintf("sensor sem release error, code: %lx\n", uwRet);
    }
  }
}
