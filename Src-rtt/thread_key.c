#include <entry.h>
#include "stm32f4xx_hal.h"
//#include "LCD.h"

#define CHK_VAL(VAL,MIN,MAX) ((VAL>MIN && VAL<MAX) ? 1 : 0)

extern ADC_HandleTypeDef hadc1;
extern uint8_t led_toggle_done;

rt_sem_t     key_sem        = RT_NULL;

rt_thread_t key_thread      = RT_NULL;
rt_thread_t key_send_thread = RT_NULL;

/* key_val = 0, none   key pressed
 * key_val = 1, right  key pressed
 * key_val = 2, up     key pressed
 * key_val = 3, down   key pressed
 * key_val = 4, left   key pressed
 * key_val = 5, select key pressed
 */
static uint8_t key_val  = 0;
static uint8_t key_last = 0;
static uint8_t disp     = 0;
void key_thread_entry(void* parameter)
{
  uint32_t adc_value = 0;
//  char buf[40];
//  char adc[40];
  while(1) {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 100);
    adc_value = HAL_ADC_GetValue(&hadc1);

    if(adc_value < 300) {
//      sprintf(buf, "right");
      key_val = 1;
    }
    if(CHK_VAL(adc_value,600,1100)) {
//      sprintf(buf, "up");
      key_val = 2;
    }
    if(CHK_VAL(adc_value,1250,1550)) {
//      sprintf(buf, "down");
      key_val = 3;
    }
    if(CHK_VAL(adc_value,1850,2150)) {
//      sprintf(buf, "left");
      key_val = 4;
    }
    if(CHK_VAL(adc_value,2350,2650)) {
//      sprintf(buf, "select");
      key_val = 5;
    }
    if(CHK_VAL(adc_value,2800,3100)) {
//      sprintf(buf, "none");
      key_val = 0;
    }

//    key_val = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

    if(key_last != key_val) {
      disp = 1;
      key_last = key_val;
      if(1 == key_val) {
        rt_err_t uwRet = RT_EOK;
        uwRet =  rt_sem_release(key_sem);
        if(RT_EOK != uwRet) {
          rt_kprintf("rt_sem_release error, code: %lx\n", uwRet);
        } else {
          led_toggle_done = 0;
        }
      }
    }
    
//    sprintf(adc, "adc: %d", adc_value);
//    LCD_Puts(0,0,buf);
    
    static uint32_t count = 0;
    if(count > 500 || disp == 1){
//      LCD_Puts(0,1,adc);
      count = 0;
      disp  = 0;
    }
    count++;
    rt_thread_delay(1);
  }
}
