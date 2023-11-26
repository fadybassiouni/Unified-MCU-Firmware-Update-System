/**
 * @file main.c
 * @author Abdelrahman Amin
 * 				Amr Khaled
 * 				ElAmir Galal
 * 				Fady Taha
 * 				Mohammed Hassan
 * 				Mohammed Reda
 * @brief This file contains functions related to Ultrasonic sensor operations.
 * @version 0.3
 * @date 2023-11-8
 * @copyright Copyright (c) 2023
 * */
#include <stdint.h>
#include "LBIT_math.h"
#include "LSTD_types.h"
#include "MRCC_interface.h"
#include "MGPIO_Interface.h"
#include "MNVIC_interface.h"
#include "MTimer_Interface.h"
#include "MSTK_interface.h"
#include "HLED_Interface.h"
#include "HUltraSonic_Interface.h"
#include "EXTI_Interface.h"


/*Jump to app pointer*/
typedef void (*Function_t)(void);
Function_t addr_to_call = 0;
volatile u8 BL_Flag ;
void EXTI7_CallbackFunc(void)
{

	BL_Flag = 1; /* Set Flag */

}
void Jump_BT(void)
{
#define SCB_VTOR   *((volatile u32*)0xE000ED08)

	SCB_VTOR = 0x08000000;  /*new address for flash code App code */

	MNVIC_stderrDisableIRQ(EXTI9_5);
	addr_to_call = *(Function_t*)(0x08000004);
	addr_to_call();

}

int main(void){

	volatile f32 Distance = 0 ;
	MRCC_stderrInitSystemClock(MRCC_SYSCLOCK_HSE_CRYSTAL);
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOA);
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOB);
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOC);
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_TIM3);
//	HLED_voidLedInit(HLED_PIN_B0);
	HLED_voidLedInit(HLED_PIN_A8);
//	HLED_voidLedInit(HLED_PIN_B3);
//	HLED_voidLedInit(HLED_PIN_B4);
//	HLED_voidLedInit(HLED_PIN_B5);
//	HLED_voidLedInit(HLED_PIN_B6);
//	HLED_voidLedInit(HLED_PIN_B7);
//	HLED_voidLedInit(HLED_PIN_B8);
//	HLED_voidLedInit(HLED_PIN_B9);
	MGPIO_stderrSetPinMode(MGPIO_PIN_A7, MGPIO_MODE_INPUT_FLOATING); /* EXTI pin */
	MSTK_stderrInit(MSTK_CLOCKSRC_AHB_8);
	MGPIO_stderrSetPinMode(MGPIO_PIN_A10, MGPIO_MODE_OUTPUT_PUSHPULL_2MHZ);
	MNVIC_stderrEnableIRQ(EXTI9_5);
	HUltraSonic_voidUltraSonicInit();
	/* Enable EXTI7 */
	EXTI_u8ConfigLine(EXTI7_LINE, RISING_EDGE_TRIGGERING);
	EXTI_u8IntEnable(EXTI7_LINE);
	EXTI_voidSetCallBackFn(EXTI7_LINE, &EXTI7_CallbackFunc);
	while(1)
	{

		Distance = HUltraSonic_voidgetDistance();
		if( BL_Flag == 1 )
		{
			BL_Flag = 0 ;
			Jump_BT(); /* Call reset handler of BL */
		}
		if(Distance < 10 )
		{

			HLED_voidLedOn(HLED_PIN_A8);
			MGPIO_stderrSetPinState(MGPIO_PIN_A10,MGPIO_HIGH);
			MSTK_voidBusyWait(200000);
			HLED_voidLedOff(HLED_PIN_A8);
			MGPIO_stderrSetPinState(MGPIO_PIN_A10,MGPIO_LOW);
			MSTK_voidBusyWait(200000);
		}
		else if(Distance<15)
		{
			HLED_voidLedOn(HLED_PIN_A8);
			MGPIO_stderrSetPinState(MGPIO_PIN_A10,MGPIO_HIGH);
			MSTK_voidBusyWait(400000);
			HLED_voidLedOff(HLED_PIN_A8);
			MGPIO_stderrSetPinState(MGPIO_PIN_A10,MGPIO_LOW);
			MSTK_voidBusyWait(400000);
		}
		else if(Distance<20)
		{
			HLED_voidLedOn(HLED_PIN_A8);
			MGPIO_stderrSetPinState(MGPIO_PIN_A10,MGPIO_HIGH);
			MSTK_voidBusyWait(800000);
			HLED_voidLedOff(HLED_PIN_A8);
			MGPIO_stderrSetPinState(MGPIO_PIN_A10,MGPIO_LOW);
			MSTK_voidBusyWait(800000);
		}
		else if(Distance<25)
		{
			HLED_voidLedOn(HLED_PIN_A8);
			MGPIO_stderrSetPinState(MGPIO_PIN_A10,MGPIO_HIGH);
			MSTK_voidBusyWait(1000000);
			HLED_voidLedOff(HLED_PIN_A8);
			MGPIO_stderrSetPinState(MGPIO_PIN_A10,MGPIO_LOW);
			MSTK_voidBusyWait(1000000);
		}
		else
		{
			MGPIO_stderrSetPinState(MGPIO_PIN_A10,MGPIO_LOW);
			HLED_voidLedOff(HLED_PIN_A8);
		}
	}
	return 0 ;

}



