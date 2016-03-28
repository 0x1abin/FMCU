#inlcude "led_driver.h"

#define MACRO_ABS(x)	((x>0)?x:-x)

static LED_STATE_T ledsList[NUMBER_OF_LEDS];

/**
  * @brief  Reset the LED.
  * @param  led: which LED.
  * @retval None.
  */
void LED_Reset(LED_NAME_T led)
{
	memset(&(ledsList[led]), 0, sizeof(LED_STATE_T));
}

/**
  * @brief  Initialize the LED.
  * @param  led: which LED.
  * @param  *pwmDutySet(): Set the LED PWM duty control func.
  * @retval None.
  */
void LED_Init(LED_NAME_T led, void (*pwmDutySet)(uint8_t duty))
{
	LED_Reset(led);
	ledsList[led].SetDutyOfPWM = pwmDutySet;
}

/**
  * @brief  Get the LED brightness.
  * @param  led: which LED.
  * @retval Brightness.
  */
uint8_t LED_GetBrightness(LED_NAME_T led)
{
	return (ledsList[led].brightness);
}

/**
  * @brief  Set the LED brightness.
  * @param  led:   which LED.
  * @param  hDuty: Duty.
  * @retval None.
  */
void LED_SetTimerOff(LED_NAME_T led, uint16_t tms)
{
	ledsList[led].bState |= LED_TIMER_MASK;
	ledsList[led].ticksTimeout = tms;
}

/**
  * @brief  Set the LED brightness.
  * @param  led:   which LED.
  * @param  hDuty: Duty.
  * @retval None.
  */
void LED_SetBrightness(LED_NAME_T led, uint8_t hDuty)
{
	ledsList[led].brightness = hDuty;
	ledsList[led].dimming.targetLevel = hDuty;
	ledsList[led].dimming.currentLevel = hDuty;
	ledsList[led].SetDutyOfPWM(ledsList[led].brightness);
}

/**
  * @brief  Set the LED blink param.
  * @param  led: which LED.
  * @param  led: blink duty(ON/OFF scale).
  * @param  led: blink period.
  * @retval None.
  */
void LED_SetBlink(LED_NAME_T led, float duty, uint16_t period)
{
	ledsList[led].blink.count = period;
	ledsList[led].blink.period = period;
	ledsList[led].blink.pulse = (uint16_t)(duty * period);
	ledsList[led].bState = (ledsList[led].bState & 0xf0) | LED_BLINK_MASK;
}

/**
  * @brief  Set the LED dimming param.
  * @param  led: which LED.
  * @retval None.
  */
void LED_Dimming(LED_NAME_T led, int8_t targetLevel, float duration)	//0~100%, duration*10ms
{	
	uint16_t durationTicks = duration * 100;
	
	float dimSpeed = (float)((int16_t)targetLevel - ledsList[led].dimming.currentLevel) / (durationTicks);
	pc.printf("dimSpeed = %f,\r\n",dimSpeed);
	
	if(MACRO_ABS(dimSpeed) < 1)
	{
		ledsList[led].dimming.incDecVal = ((dimSpeed > 0)?1:-1);
		ledsList[led].dimming.ticksCountReload = ((float)1.0/dimSpeed);
	}
	else
	{
		//for current % dimspeed == 0
		while((MACRO_ABS((int16_t)ledsList[led].dimming.targetLevel - 
				ledsList[led].dimming.currentLevel)) % (uint8_t)dimSpeed != 0)
		{
			ledsList[led].dimming.currentLevel++;
		}
		
		ledsList[led].dimming.incDecVal = (int8_t)dimSpeed;
		ledsList[led].dimming.ticksCountReload = 1;
	}
	
	ledsList[led].dimming.targetLevel = targetLevel;
	ledsList[led].dimming.ticksCount = ledsList[led].dimming.ticksCountReload;
	ledsList[led].bState = (ledsList[led].bState & 0xf0) | LED_DIMMING_MASK;
}

/**
  * @brief  LED ctrl process func,10ms period call this func.
  * @param  led: which LED.
  * @retval None.
  */
void LED_Task_Ticks()
{
	uint8_t index;
	static uint8_t prevBrightness[NUMBER_OF_LEDS];
	
	for(index=0; index<NUMBER_OF_LEDS; index++)
	{
		if((ledsList[index].bState & LED_TIMER_MASK) != 0)
		{
			if(--ledsList[index].ticksTimeout == 0)
			{
				ledsList[index].bState = 0;
				ledsList[index].brightness = 0;
			}
		}
		
		if((ledsList[index].bState & 0X0f) == LED_DIMMING_MASK)
		{
			//dimming process
			if(ledsList[index].dimming.currentLevel != ledsList[index].dimming.targetLevel)
			{
				if(--ledsList[index].dimming.ticksCount <= 0)
				{
					ledsList[index].dimming.ticksCount = ledsList[index].dimming.ticksCountReload;
					ledsList[index].dimming.currentLevel = (uint8_t)(ledsList[index].dimming.incDecVal + 
																								 (int8_t)ledsList[index].dimming.currentLevel);
					ledsList[index].brightness = ledsList[index].dimming.currentLevel;
				}
			}
		}
		else if((ledsList[index].bState & 0x0f) == LED_BLINK_MASK)
		{
			//blink process
			if(--ledsList[index].blink.count <= 0)
			{
				ledsList[index].blink.count = ledsList[index].blink.period;
			}
			
			if(ledsList[index].blink.count < ledsList[index].blink.pulse)
			{
				ledsList[index].brightness = ledsList[index].dimming.currentLevel;
			}
			else
			{
				ledsList[index].brightness = 0;
			}
		}
		
		//have changer
		if(prevBrightness[index] != ledsList[index].brightness)
		{
			ledsList[index].SetDutyOfPWM(ledsList[index].brightness);
			prevBrightness[index] = ledsList[index].brightness;
		}
		
	}
}

