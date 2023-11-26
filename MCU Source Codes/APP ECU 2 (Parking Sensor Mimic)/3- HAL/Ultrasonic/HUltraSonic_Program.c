/**
 * @file HUltraSonic.c
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


#include "LBIT_math.h"
#include "LSTD_types.h"
#include "MRCC_interface.h"
#include "MGPIO_Interface.h"
#include "MNVIC_interface.h"
#include "MTimer_Interface.h"
#include "MSTK_interface.h"
#include "MTimer_Private.h"
#include "HUltraSonic_Interface.h"
#include "HLED_Interface.h"
volatile u8 Flag = 0 ;
volatile u32 ICU1 ,ICU2 ;

/**
 * @brief Initializes the Ultrasonic sensor.
 * Configures timer, GPIO pins, and interrupts.
 */
void HUltraSonic_voidUltraSonicInit()
{
	MGPIO_stderrSetPinMode(MGPIO_PIN_A0, MGPIO_MODE_OUTPUT_PUSHPULL_2MHZ);
	MGPIO_stderrSetPinMode(MGPIO_PIN_A6, MGPIO_MODE_INPUT_FLOATING);
	MTIM3_stderrTimerInit(MTIM_CHANNEL1,0);
	MTIM3_stderrEdgeTrigger(MTIM_CHANNEL1,MTIM_EDGE_TRIGGER_RISING);
	MTIM3_stderrEnableICU(MTIM_CHANNEL1);
	MTIM3_stderrSetCallBack(UltraSonic);
	MTIM3_stderrInterruptEnable(MTIM_INT_CC1);
}

/**
 * @brief Measures the distance using the Ultrasonic sensor.
 * @return The measured distance in centimeters.
 */
f32 HUltraSonic_voidgetDistance()
{
	Flag=0;
	volatile f32   Distance =0 ;
	volatile u32  UltraSonicTimer ;
	MTIM3_voidResetCounter();
	MTIM3_voidTimerStop();
	MNVIC_stderrEnableIRQ(29);
	MGPIO_stderrSetPinState(MGPIO_PIN_A0, MGPIO_LOW);
	MGPIO_stderrSetPinState(MGPIO_PIN_A0, MGPIO_HIGH) ;
	MSTK_voidBusyWait(20);
	MGPIO_stderrSetPinState(MGPIO_PIN_A0, MGPIO_LOW);
	MTIM3_voidTimerStart();
	while(Flag<2);

	if(ICU1>ICU2)
	{
		UltraSonicTimer = ICU2+65536 - ICU1;
		Flag = 0 ;
		Distance = UltraSonicTimer/58 ;
		MTIM3_voidTimerStop();
//		OverFlows=0;
	}
	else if (ICU1<ICU2 )
	{
		UltraSonicTimer = ICU2- ICU1;
		Flag = 0 ;
		Distance = UltraSonicTimer/58 ;
		MTIM3_voidTimerStop();
//		OverFlows=0;

	}

	return Distance/8 ;

}

/**
 * @brief Interrupt service routine for Ultrasonic sensor measurements.
 */
static void UltraSonic(void)
{


		if(Flag==0)
		{
			ICU1 = MTIM3_u16GetCapturedValue(MTIM_CHANNEL1);
			MTIM3_stderrEdgeTrigger(MTIM_CHANNEL1,MTIM_EDGE_TRIGGER_FALLING);
			Flag=1 ;
		}
		else if (Flag==1)
		{

			ICU2 = MTIM3_u16GetCapturedValue(MTIM_CHANNEL1);
			Flag = 2 ;

			MTIM3_stderrEdgeTrigger(MTIM_CHANNEL1,MTIM_EDGE_TRIGGER_RISING);
			MNVIC_stderrDisableIRQ(29);
		}


	}

