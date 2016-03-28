#ifndef _LED_DRIVER_H_
#define _LED_DRIVER_H_

#include <ZW_pindefs.h>
#include <ZW_typedefs.h>
#include <ZW_basis_api.h>
#include <ZW_led_api.h>

#define uint8			BYTE
#define uint16		WORD
#define BOOL			bit

#define LED_TIMER_MASK			0X80
#define LED_DIMMING_MASK		0X01
#define LED_BLINK_MASK			0X02

typedef enum
{
  xLED1 = 0,
	xLED2,
	xLED3,
	xLED4,
	xLED5,
  NUMBER_OF_LEDS	//!< NUMBER_OF_LEDS
}LED_NAME_T;

typedef struct __LED_BLINK_T_
{
	uint16_t count;
	uint16_t period;
	uint16_t pulse;
}LED_BLINK_T;

typedef struct __LED_DIMMING_T_
{
	uint8_t targetLevel;
	uint8_t currentLevel;
	uint16_t ticksCount;
	uint16_t ticksCountReload;
	int8_t incDecVal;
}LED_DIMMING_T;

typedef struct __LED_STATE_T_
{
	uint8_t bState;
	uint8_t brightness;
	uint16_t ticksTimeout;
	LED_BLINK_T blink;
	LED_DIMMING_T dimming;
	void (*SetDutyOfPWM)(uint8_t hDuty);
}LED_STATE_T;


extern void LED_Reset(LED_NAME_T led);
extern void LED_Init(LED_NAME_T led, void (*pwmDutySet)(uint8_t duty));
extern void LED_SetTimerOff(LED_NAME_T led, uint16_t tms);
extern void LED_SetBrightness(LED_NAME_T led, uint8_t hDuty);
extern uint8_t LED_GetBrightness(LED_NAME_T led);
extern void LED_SetBlink(LED_NAME_T led, float duty, uint16_t period);
extern void LED_Dimming(LED_NAME_T led, int8_t targetLevel, float duration);
extern void LED_Task_Ticks();


#endif