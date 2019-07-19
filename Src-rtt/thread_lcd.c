#include <entry.h>
#include "stm32f4xx_hal.h"
#include "LCD.h"
#include "dht11.h"

extern DHT11_Dev dht11;
extern float light_lx;
extern uint8_t current_mode;

rt_thread_t lcd_thread = RT_NULL;

static void lcd_disp(char* line1, char* line2);

void lcd_thread_entry(void* parameter)
{
  char line1[16];
  char line2[16];
  while(1) {    
    sprintf(line1, "Light: %.2fLX", light_lx);
    sprintf(line2, "Mode : %d", current_mode);
    lcd_disp(line1, line2);
    
    sprintf(line1, "RH  : %d.%d%%", dht11.humidity, dht11.hum_float);
    sprintf(line2, "TEMP: %d.%d ", dht11.temparature, dht11.temp_float);
    lcd_disp(line1, line2);
  }
}
    
static void lcd_disp(char* line1, char* line2)
{
  LCD_Puts(0,0,line1);
  LCD_Puts(0,1,line2);
  
  rt_thread_delay(3000);
}
  