/*Libraries*/
#include <LSTD_types.h>
#include <LBIT_math.h>
#include <stdio.h>

/*MCAL*/
#include "MRCC_interface.h"
#include "MGPIO_interface.h"
#include "MSTK_interface.h"
#include "MADC_interface.h"
#include "MTIM1_interface.h"
#include "MUART_interface.h"
#include "NVIC_Interface.h"
#include "EXTI_Interface.h"



int map(int value, int inMin, int inMax, int outMin, int outMax);

/*Jump to app pointer*/
typedef void (*Function_t)(void);
Function_t addr_to_call = 0;
volatile u8 BL_Flag ; /* To check the signal of EXTI */


void EXTI4_CallbackFunc(void){

	BL_Flag = 1; /* Set Flag */
	EXTI_u8IntDisable(EXTI4_LINE);
	NVIC_u8DisableINT(EXTI4_IRQ);


}

void Jump_BT(void)
{
#define SCB_VTOR   *((volatile u32*)0xE000ED08)

	SCB_VTOR = 0x08000000;  /*  return to Base address */


	addr_to_call = *(Function_t*)(0x08000004);
	addr_to_call();

}


int main(void)
{
	/*System Clock Initialization*/
	MRCC_stderrInitSystemClock(MRCC_SYSCLOCK_HSE_CRYSTAL);

	/*System Buses Clock Initialization*/
	MRCC_stderrInitBusClock(MRCC_BUS_APB2, MRCC_APB2_NO_PRESCALAR);
	MRCC_stderrInitBusClock(MRCC_BUS_APB1, MRCC_APB1_NO_PRESCALAR);

	/*Enable Clock Security System*/
	MRCC_voidEnableCSS();

	/*Enable Clock For Peripherals*/
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOA);
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_USART2);
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_ADC1);
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_TIM1);

	/*Timer 1 AF GPIO Mode Change*/
	MGPIO_stderrSetPinMode(MGPIO_PIN_A8, MGPIO_MODE_AF_PUSHPULL_2MHZ);

	/*Timer 1 Initialization*/
	MTIM1_stderrInit(65535, 0);
	MTIM1_voidEnable();

	/*UART2 AF GPIO Mode Change*/
	MGPIO_stderrSetPinMode(MGPIO_PIN_A2, MGPIO_MODE_AF_PUSHPULL_2MHZ);
	MGPIO_stderrSetPinMode(MGPIO_PIN_A3, MGPIO_MODE_INPUT_FLOATING);

	MGPIO_stderrSetPinMode(MGPIO_PIN_A4, MGPIO_MODE_INPUT_FLOATING); /* EXTI pin */
	MGPIO_stderrSetPinMode(MGPIO_PIN_A1, MGPIO_MODE_OUTPUT_PUSHPULL_2MHZ); /* Led pin */

	/*UART2 Initialization*/
	MUART_stderrInit(MUART_UART2, 0, MUART_DATABITS_8, MUART_PARITY_NONE, MUART_STOPBITS_1);
	MUART_voidEnableTx(MUART_UART2);
	MUART_voidEnableRx(MUART_UART2);

	/* NVIC Init  */
	NVIC_u8EnableINT(EXTI4_IRQ);

	/* Enable EXTI7 */
	EXTI_u8ConfigLine(EXTI4_LINE, RISING_EDGE_TRIGGERING);
	EXTI_u8IntEnable(EXTI4_LINE);
	EXTI_voidSetCallBackFn(EXTI4_LINE, &EXTI4_CallbackFunc);

	/*Initialize SysTick*/
	MSTK_stderrInit(MSTK_CLOCKSRC_AHB_8);

	/*Initialize ADC*/
	MGPIO_stderrSetPinMode(MGPIO_PIN_A0, MGPIO_MODE_INPUT_ANALOG);
	MGPIO_stderrSetPinState(MGPIO_PIN_A0, MGPIO_HIGH);


	MADC_voidInit();
	u16 L_u16ReadAdc=0;
	char dataDebug[100] = {0};

	MADC_voidEnable();
	MSTK_voidBusyWait(10000);


	for(;;)
	{
		MADC_voidStartConversion();
		MADC_voidEOCFlagPolling();
		MADC_stderrReadAdc(&L_u16ReadAdc);

		MTIM1_stderrSetDutyCycle(map(L_u16ReadAdc, 0, 1000, 0, 65535));
		sprintf(dataDebug, "ADC: %d\n", L_u16ReadAdc);
		MUART_voidSendString(MUART_UART2, dataDebug);
		if( BL_Flag == 1 )
		{
			BL_Flag = 0 ;
			Jump_BT(); /* Call reset handler of BL */
		}
		else
		{
			/* Do nothing */
		}

	}
}


int map(int value, int inMin, int inMax, int outMin, int outMax)
{
	return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}















