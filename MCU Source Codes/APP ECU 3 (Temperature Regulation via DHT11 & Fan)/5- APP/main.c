#include <LSTD_types.h>
#include <LBIT_math.h>
#include <MRCC_interface.h>
#include <MGPIO_interface.h>
#include <MSTK_interface.h>
#include <MNVIC_interface.h>
#include <EXTI_Interface.h>
#include <MUART_interface.h>
#include <MTIM1_interface.h>
#include <HDHT11_interface.h>
#include <stdlib.h>
#include <stdio.h>

#define DHT11_PIN MGPIO_PIN_A0


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
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_GPIOB);
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_TIM1);
	MRCC_stderrEnablePeripheralClock(MRCC_PERIPHERAL_USART2);

	/*Timer 1 AF GPIO Mode Change*/
	MGPIO_stderrSetPinMode(MGPIO_PIN_A8, MGPIO_MODE_AF_PUSHPULL_2MHZ);

	/*Timer 1 Initialization*/
	MTIM1_stderrInit(65535, 0);
	MTIM1_voidEnable();

	/* EXTI pin */
	MGPIO_stderrSetPinMode(MGPIO_PIN_A7, MGPIO_MODE_INPUT_FLOATING);

	/* Enable EXTI7 */
	EXTI_u8ConfigLine(EXTI7_LINE, RISING_EDGE_TRIGGERING);
	EXTI_u8IntEnable(EXTI7_LINE);
	EXTI_voidSetCallBackFn(EXTI7_LINE, &EXTI7_CallbackFunc);

	/*NVIC Enable EXTI Line 7*/
	MNVIC_stderrEnableIRQ(EXTI9_5);

	/*UART2 AF GPIO Mode Change*/
	MGPIO_stderrSetPinMode(MGPIO_PIN_A2, MGPIO_MODE_AF_PUSHPULL_2MHZ);
	MGPIO_stderrSetPinMode(MGPIO_PIN_A3, MGPIO_MODE_INPUT_FLOATING);

	/*UART2 Initialization*/
	MUART_stderrInit(MUART_UART2, 0, MUART_DATABITS_8, MUART_PARITY_NONE, MUART_STOPBITS_1);
	MUART_voidEnableTx(MUART_UART2);
	MUART_voidEnableRx(MUART_UART2);

	char debugData[100] = {0};
	HDHT11_stderrInit(DHT11_PIN);
	HDHT11_data sensorData;


	for(;;)
	{
		if(HDHT11_hdht11statusReadData(&sensorData, DHT11_PIN) == HDHT11_DATA_READY)
		{
			sprintf(debugData, "Temp: %d.%d\tHumidity: %d.%d\tChecksum: %d\n", sensorData.data.temperatureInt, sensorData.data.temperatureDec, sensorData.data.humidityInt, sensorData.data.humidityDec, sensorData.data.checksum);
			MUART_voidSendString(MUART_UART2, debugData);
		}


		if(sensorData.data.temperatureInt >= 30)
		{
			MTIM1_stderrSetDutyCycle(65535);
		}
		else if(sensorData.data.temperatureInt >= 25)
		{
			MTIM1_stderrSetDutyCycle(30000);
		}
		else if(sensorData.data.temperatureInt >= 20)
		{
			MTIM1_stderrSetDutyCycle(10000);
		}
		else
		{
			MTIM1_stderrSetDutyCycle(0);
		}

		if( BL_Flag == 1 )
		{
			BL_Flag = 0 ;
			Jump_BT(); /* Call reset handler of BL */
		}
		else
		{
			/*MISRA-C*/
		}


	}
}





