/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : NVIC               */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : September 18,2023  */
/* Last Edit : N/A                */
/**********************************/
#ifndef _MNVIC_INTERFACE_H_
#define _MNVIC_INTERFACE_H_

/*Vector Table*/
typedef enum 
{
	WWDG = 0,	
	PVD = 1,
	TAMPER = 2,
	RTC = 3,
	FLASH = 4,
	RCC = 5,	
	EXTI0 = 6, 
	EXTI1 = 7, 
	EXTI2 = 8, 
	EXTI3 = 9, 
	EXTI4 = 10, 
	DMA1_CHANNEL1 = 11,
	DMA1_CHANNEL2 = 12,
	DMA1_CHANNEL3 = 13,
	DMA1_CHANNEL4 = 14,
	DMA1_CHANNEL5 = 15, 
	DMA1_CHANNEL6 = 16,
	DMA1_CHANNEL7 = 17, 
	USB_HP_CAN_TX = 19,
	USB_HP_CAN_RX0 = 20,
	CAN_RX1 = 21,
	CAN_SCE = 22,
	EXTI9_5 = 23,
	TIM1_BRK = 24,
	TIM1_UP = 25,
	TIM1_TRG_COM = 26,
	TIM1_CC = 27,
	TIM2 = 28,
	TIM3 = 29,
	TIM4 = 30,
	I2C1_EV = 31,	
	I2C1_ER = 32,	
	I2C2_EV = 33,	
	I2C2_ER = 34,	
	SPI1 = 35,	
	SPI2 = 36,	
	USART1 = 37,	
	USART2 = 38,
	USART3 = 39,	
	EXTI15_10 = 40,
	RTC_ALARM = 41,
	USBWAKEUP = 42,
	TIM8_BRK = 43,
	TIM8_UP = 44,
	TIM8_TRG_COM = 45,
	TIM8_CC = 46,
	ADC3 = 47,
	FSMC = 48,
	SDIO = 49,
	TIM5 = 50,
	SPI3 = 51,
	UART4 = 52,
	UART5 = 53,
	TIM6 = 54,
	TIM7 = 55,
	DMA2_CHANNEL1 = 56, 
	DMA2_CHANNEL2 = 57, 
	DMA2_CHANNEL3 = 58, 
	DMA2_CHANNEL4_5 = 59, 
}VECTOR_t;

#define MNVIC_GROUPING_16G_0S 3
#define MNVIC_GROUPING_8G_2S  4
#define MNVIC_GROUPING_4G_4S  5
#define MNVIC_GROUPING_2G_8S  6
#define MNVIC_GROUPING_0G_16S 7

STD_error_t MNVIC_stderrEnableIRQ(VECTOR_t ARG_vector_tVectorID);
STD_error_t MNVIC_stderrDisableIRQ(VECTOR_t ARG_vector_tVectorID);
STD_error_t MNVIC_stderrSetPendingIRQ(VECTOR_t ARG_vector_tVectorID);
STD_error_t MNVIC_stderrClearPendingIRQ(VECTOR_t ARG_vector_tVectorID);
STD_error_t MNVIC_stderrGetPendingIRQ(u8 *ARG_pu8PendingState, VECTOR_t ARG_vector_tVectorID);
STD_error_t MNVIC_stderrGetActiveIRQ(u8 *ARG_pu8ActiveState, VECTOR_t ARG_vector_tVectorID);
STD_error_t MNVIC_stderrSetGroupingMode(u8 ARG_u8GroupingMode);
STD_error_t MNVIC_stderrSetPriority(VECTOR_t ARG_vector_tVectorID, u8 ARG_u8Priority);
STD_error_t MNVIC_stderrGetPriority(u8 *ARG_pu8Priority, VECTOR_t ARG_vector_tVectorID);

#endif
