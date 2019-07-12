#include <entry.h>
#include "stm32f4xx_hal.h"
#include "LCD.h"

#define CHK_VAL(VAL,MIN,MAX) ((VAL>MIN && VAL<MAX) ? 1 : 0)

extern ADC_HandleTypeDef hadc1;

rt_mq_t     key_mq          = RT_NULL;

rt_thread_t key_thread      = RT_NULL;
rt_thread_t key_send_thread = RT_NULL;

/* key_val = 0, none   key pressed
 * key_val = 1, right  key pressed
 * key_val = 2, up     key pressed
 * key_val = 3, down   key pressed
 * key_val = 4, left   key pressed
 * key_val = 5, select key pressed
 */
static uint8_t key_val = 0;
static uint8_t key_last = 0;

void key_thread_entry(void* parameter)
{
  uint32_t adc_value = 0;
  char buf[40];
  char adc[40];
  while(1) {
    HAL_ADC_Start(&hadc1);
    adc_value = HAL_ADC_GetValue(&hadc1);
    adc_value = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

    if(adc_value < 300) {
      sprintf(buf, "right");
      key_val = 1;
    }
    if(CHK_VAL(adc_value,600,900)) {
      sprintf(buf, "up");
      key_val = 2;
    }
    if(CHK_VAL(adc_value,1250,1550)) {
      sprintf(buf, "down");
      key_val = 3;
    }
    if(CHK_VAL(adc_value,1850,2150)) {
      sprintf(buf, "left");
      key_val = 4;
    }
    if(CHK_VAL(adc_value,2350,2650)) {
      sprintf(buf, "select");
      key_val = 5;
    }
    if(CHK_VAL(adc_value,2800,3100)) {
      sprintf(buf, "none");
      key_val = 0;
    }

    key_val = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

    sprintf(adc, "adc: %d", adc_value);
    LCD_Puts(0,0,buf);
    LCD_Puts(0,1,adc);
    rt_thread_delay(10);
  }
}

void key_send_entry(void* parameter)
{
  rt_err_t uwRet = RT_EOK;
  while(1) {
    if(key_last != key_val) {
      key_last = key_val;
      uwRet = rt_mq_send(key_mq,
                         &key_val,
                         sizeof(key_val));
      if(RT_EOK != uwRet) {
        rt_kprintf("data can not send to message queue, code: %lx\n", uwRet);
      }
    }
    rt_thread_delay(10);
  }
}
