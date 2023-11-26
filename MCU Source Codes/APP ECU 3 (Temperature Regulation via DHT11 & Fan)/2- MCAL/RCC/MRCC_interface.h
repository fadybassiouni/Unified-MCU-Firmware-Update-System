/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : RCC                */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : September 12,2023  */
/* Last Edit : N/A                */
/**********************************/

#ifndef _MRCC_INTERFACE_H_
#define _MRCC_INTERFACE_H_

/*Configuation Structure For PLL*/
typedef struct
{	
	u8  PLLClockSource;
	u8  HSEPrescalar;
	u16 MultiplicationFactor;
	u8  USBClockPrescalar;
}MRCC_PLLconfig_t;

/*System Clock Config Defines*/
#define MRCC_SYSCLOCK_HSI 			0
#define MRCC_SYSCLOCK_HSE_CRYSTAL   1
#define MRCC_SYSCLOCK_PLL 			2
#define MRCC_SYSCLOCK_HSE_USER_DEF  3



/******************************************
***********System Clock Options************
*******************************************
* 1- MRCC_SYSCLOCK_HSI                    *
* 2- MRCC_SYSCLOCK_HSE_CRYSTAL            *
* 3- MRCC_SYSCLOCK_HSE_USER_DEF           *
* 4- MRCC_SYSCLOCK_PLL                    *
*******************************************/
STD_error_t MRCC_stderrInitSystemClock(u8 ARG_u8SystemClockSource);


/*RCC PLL Config Defines*/
#define MRCC_PLL_SRC_HSI_2 				 0
#define MRCC_PLL_SRC_HSE  		         1
#define MRCC_PLL_SRC_HSE_2               1

#define MRCC_USB_PRE_1_5                 0
#define MRCC_USB_NO_PRE                  1



/******************************************
*************PLL Config Options************
*******************************************
* -PLL CLock Source-					  *
* 1- MRCC_PLL_SRC_HSI_2				      *
* 2- MRCC_PLL_SRC_HSE  		              *
*	                                      *
* -HSE input Prescaler		              *					
* MIN: 1                                  *
* MAX: 2	                         	  *
* 	                                      * 
*                                         *
* -PLL Multiplication Factor-			  *
* MIN: 2 -> input 0	      				  *
* MAX: 16 -> input 16					  *
* 										  *
* -PLL USB Prescalar Factor-			  *
* MIN: 1 								  *
* MAX: 1.5								  *	
*******************************************/
STD_error_t MRCC_stderrConfigurePLL(MRCC_PLLconfig_t *ARG_pmrcc_pllconfig_tConfig);


/*RCC Bus Clocks Config Defines*/
#define MRCC_BUS_AHB                    0
#define MRCC_BUS_APB1                   1
#define MRCC_BUS_APB2                   2

#define MRCC_AHB_PRESCALAR_2            8
#define MRCC_AHB_PRESCALAR_4            9
#define MRCC_AHB_PRESCALAR_8            10
#define MRCC_AHB_PRESCALAR_16           11
#define MRCC_AHB_PRESCALAR_64           12
#define MRCC_AHB_PRESCALAR_128          13
#define MRCC_AHB_PRESCALAR_256          14
#define MRCC_AHB_PRESCALAR_512          15

#define MRCC_APB1_NO_PRESCALAR           3
#define MRCC_APB1_PRESCALAR_2            4
#define MRCC_APB1_PRESCALAR_4            5
#define MRCC_APB1_PRESCALAR_8            6
#define MRCC_APB1_PRESCALAR_16           7

#define MRCC_APB2_NO_PRESCALAR           3
#define MRCC_APB2_PRESCALAR_2            4
#define MRCC_APB2_PRESCALAR_4            5
#define MRCC_APB2_PRESCALAR_8            6
#define MRCC_APB2_PRESCALAR_16           7


/******************************************
*****************Bus Options***************
*******************************************
* -Buses-                                 *
* 1- MRCC_BUS_AHB	                      *
* 2- MRCC_BUS_APB1                        *
* 3- MRCC_BUS_APB2 						  *
* 										  *
* -Prescalers-							  *
* MRCC_AHB_PRESCALAR_2                    *
* MRCC_AHB_PRESCALAR_4                    *
* MRCC_AHB_PRESCALAR_8                    *
* MRCC_AHB_PRESCALAR_16                   *
* MRCC_AHB_PRESCALAR_64                   *
* MRCC_AHB_PRESCALAR_128                  *
* MRCC_AHB_PRESCALAR_256                  *
* MRCC_AHB_PRESCALAR_512                  *
* MRCC_APB1_NO_PRESCALAR                  *
* MRCC_APB1_PRESCALAR_2                   *
* MRCC_APB1_PRESCALAR_4                   *
* MRCC_APB1_PRESCALAR_8                   *
* MRCC_APB1_PRESCALAR_16                  *
* MRCC_APB2_NO_PRESCALAR                  *
* MRCC_APB2_PRESCALAR_2                   *
* MRCC_APB2_PRESCALAR_4                   *
* MRCC_APB2_PRESCALAR_8                   *
* MRCC_APB2_PRESCALAR_16                  *	
*******************************************/
STD_error_t MRCC_stderrInitBusClock(u8 ARG_u8Bus, u8 ARG_u8ClockPrescalar);


/******************************************
****************Peripheral IDs*************
*******************************************/
#define MRCC_PERIPHERAL_DMA1             0
#define MRCC_PERIPHERAL_DMA2             1
#define MRCC_PERIPHERAL_SRAM             2
#define MRCC_PERIPHERAL_FLITF            4
#define MRCC_PERIPHERAL_CRC              6
#define MRCC_PERIPHERAL_FSMC             8
#define MRCC_PERIPHERAL_AFIO             32
#define MRCC_PERIPHERAL_GPIOA            34
#define MRCC_PERIPHERAL_GPIOB            35
#define MRCC_PERIPHERAL_GPIOC            36
#define MRCC_PERIPHERAL_ADC1             41
#define MRCC_PERIPHERAL_ADC2             42
#define MRCC_PERIPHERAL_TIM1             43
#define MRCC_PERIPHERAL_SPI1             44
#define MRCC_PERIPHERAL_TIM8             45
#define MRCC_PERIPHERAL_USART1           46
#define MRCC_PERIPHERAL_ADC3             47
#define MRCC_PERIPHERAL_TIM9             51
#define MRCC_PERIPHERAL_TIM10            52
#define MRCC_PERIPHERAL_TIM11            53
#define MRCC_PERIPHERAL_TIM2             64
#define MRCC_PERIPHERAL_TIM3             65
#define MRCC_PERIPHERAL_TIM4             66
#define MRCC_PERIPHERAL_TIM5             67
#define MRCC_PERIPHERAL_TIM6             68
#define MRCC_PERIPHERAL_TIM7             69
#define MRCC_PERIPHERAL_TIM12            70
#define MRCC_PERIPHERAL_TIM13            71
#define MRCC_PERIPHERAL_TIM14            72
#define MRCC_PERIPHERAL_WWDG             75
#define MRCC_PERIPHERAL_SPI2             78
#define MRCC_PERIPHERAL_SPI3             79
#define MRCC_PERIPHERAL_USART2           81
#define MRCC_PERIPHERAL_USART3           82
#define MRCC_PERIPHERAL_UART4            83
#define MRCC_PERIPHERAL_UART5            84
#define MRCC_PERIPHERAL_I2C1             85
#define MRCC_PERIPHERAL_I2C2             86
#define MRCC_PERIPHERAL_USB              87
#define MRCC_PERIPHERAL_CAN              89
#define MRCC_PERIPHERAL_BKP              91
#define MRCC_PERIPHERAL_PWR              92
#define MRCC_PERIPHERAL_DAC              93

STD_error_t MRCC_stderrEnablePeripheralClock(u8 ARG_u8PeripheralID);
STD_error_t MRCC_stderrDisablePeripheralClock(u8 ARG_u8PeripheralID);

/*Clock Security System*/
void MRCC_voidEnableCSS(void);
void MRCC_voidDisableCSS(void);


#endif
