#include "dht11.h"
TIM_HandleTypeDef ustim;

int DHT11_Init(struct DHT11_Dev* dev,TIM_TypeDef *TIMx, GPIO_TypeDef* port, uint16_t pin) {
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	dev->port = port;
	dev->pin = pin;
	
	if(TIMx==TIM1)
	__HAL_RCC_TIM1_CLK_ENABLE();
	else if(TIMx==TIM2)
	__HAL_RCC_TIM2_CLK_ENABLE();	
	else if(TIMx==TIM3)
	__HAL_RCC_TIM3_CLK_ENABLE();	
	else if(TIMx==TIM4)
	__HAL_RCC_TIM4_CLK_ENABLE();	
		
		

	//Initialize TIMER
	TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

	
  ustim.Instance = TIMx;
  ustim.Init.Prescaler = 84;
  ustim.Init.CounterMode = TIM_COUNTERMODE_UP;
  ustim.Init.Period = 84000000-1;
  ustim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//	ustim.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&ustim) != HAL_OK)
  {
//    _Error_Handler(__FILE__, __LINE__);
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&ustim, &sClockSourceConfig) != HAL_OK)
  {
//    _Error_Handler(__FILE__, __LINE__);
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&ustim, &sMasterConfig) != HAL_OK)
  {
//    _Error_Handler(__FILE__, __LINE__);
  }
	HAL_TIM_Base_Start(&ustim);
	
	//Initialize GPIO DHT11
	GPIO_InitStructure.Pin = dev->pin;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(dev->port, &GPIO_InitStructure);

	return 0;
}

int DHT11_Read(struct DHT11_Dev* dev) {
	
	//Initialization
	uint8_t i, j, temp;
	uint8_t data[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//Generate START condition
	//o
	GPIO_InitStructure.Pin = dev->pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(dev->port, &GPIO_InitStructure);
	
	//dev->port->MODER |= GPIO_MODER_MODER6_0;
	
	//Put LOW for at least 18ms
	HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_RESET);
	
	
	//wait 18ms
	
	
	ustim.Instance->CNT=0;
	while((ustim.Instance->CNT) <= 18000);
	
	//Put HIGH for 20-40us
	HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_SET);
	
	//wait 40us
	ustim.Instance->CNT=0;
	while((ustim.Instance->CNT) <= 40);
	//End start condition
	
	//io();
	//Input mode to receive data
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(dev->port, &GPIO_InitStructure);
	
	//DHT11 ACK
	//should be LOW for at least 80us
	//while(!GPIO_ReadInputDataBit(dev->port, dev->pin));
	ustim.Instance->CNT=0;
	while(!HAL_GPIO_ReadPin(dev->port, dev->pin)) {
		if(ustim.Instance->CNT > 100)
			return DHT11_ERROR_TIMEOUT;
	}
	
	//should be HIGH for at least 80us
	//while(GPIO_ReadInputDataBit(dev->port, dev->pin));		
	ustim.Instance->CNT=0;
	while(HAL_GPIO_ReadPin(dev->port, dev->pin)) {
		if(ustim.Instance->CNT > 100)
			return DHT11_ERROR_TIMEOUT;
	}
	
	//Read 40 bits (8*5)
	for(j = 0; j < 5; ++j) {
		for(i = 0; i < 8; ++i) {
			
			//LOW for 50us
			while(!HAL_GPIO_ReadPin(dev->port, dev->pin));
			
			
			//Start counter
			ustim.Instance->CNT=0;
			
			//HIGH for 26-28us = 0 / 70us = 1
			while(HAL_GPIO_ReadPin(dev->port, dev->pin));
			
			
			//Calc amount of time passed
			temp = ustim.Instance->CNT;
			
			//shift 0
			data[j] = data[j] << 1;
			
			//if > 30us it's 1
			if(temp > 40)
				data[j] = data[j]+1;
		}
	}
	
	//verify the Checksum
	if(data[4] != (data[0] + data[1] + data[2] + data[3]))
		return DHT11_ERROR_CHECKSUM;
	
	//set data
	dev->humidity    = data[0];
	dev->hum_float   = data[1];
	dev->temparature = data[2];
	dev->temp_float  = data[3];
	
	return DHT11_SUCCESS;
}
