#include <entry.h>
#include "stm32f4xx_hal.h"
#include "dht11.h"

rt_thread_t dht11_thread = RT_NULL;

DHT11_Dev dht11;

void dht11_thread_entry(void* parameter)
{
  char rh[40];
  char temp[40];
  while(1) {    
    DHT11_Read(&dht11);
    rt_thread_delay(10);
  }
}
