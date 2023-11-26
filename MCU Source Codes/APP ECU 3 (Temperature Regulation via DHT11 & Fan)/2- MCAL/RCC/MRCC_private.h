/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : RCC                */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : September 12,2023  */
/* Last Edit : N/A                */
/**********************************/

#ifndef _MRCC_PRIVATE_H_
#define _MRCC_PRIVATE_H_

/*BASE ADDRESS*/
#define MRCC_BASE_ADDRESS ((volatile u32*)(0x40021000))

/*RCC Register Structure*/
typedef struct 
{
	volatile u32 CR;
	volatile u32 CFGR;
	volatile u32 CIR;
	volatile u32 APB2RSTR;
	volatile u32 APB1RSTR;
	volatile u32 AHBENR;
	volatile u32 APB2ENR;
	volatile u32 APB1ENR;
	volatile u32 BDCR;
}MRCC_t;

#define MRCC ((MRCC_t *)MRCC_BASE_ADDRESS)
#define MRCC_AHBENR ((volatile u32 *)(0x40021014))
#define MRCC_APB2ENR ((volatile u32 *)(0x40021018))
#define MRCC_APB1ENR ((volatile u32 *)(0x4002101C))

/*RCC Control Register Bit Definitions*/
#define MRCC_CR_HSION                     0
#define MRCC_CR_HSIRDY                    1
#define MRCC_CR_HSITRIM0                  3
#define MRCC_CR_HSITRIM1                  4
#define MRCC_CR_HSITRIM2                  5
#define MRCC_CR_HSITRIM3                  6
#define MRCC_CR_HSITRIM4                  7
#define MRCC_CR_HSICAL0                   8
#define MRCC_CR_HSICAL1                   9
#define MRCC_CR_HSICAL2                   10   
#define MRCC_CR_HSICAL3                   11
#define MRCC_CR_HSICAL4                   12
#define MRCC_CR_HSICAL5                   13
#define MRCC_CR_HSICAL6                   14
#define MRCC_CR_HSICAL7                   15
#define MRCC_CR_HSEON                     16
#define MRCC_CR_HSERDY                    17
#define MRCC_CR_HSEBYP                    18
#define MRCC_CR_CSSON                     19
#define MRCC_CR_PLLON                     24
#define MRCC_CR_PLLRDY                    25


/*RCC Clock Configuration Register Bit Definitions*/
#define MRCC_CFGR_SW0     		          0
#define MRCC_CFGR_SW1     		          1
#define MRCC_CFGR_SWS0    		          2
#define MRCC_CFGR_SWS1    		          3
#define MRCC_CFGR_HPRE0   		          4
#define MRCC_CFGR_HPRE1   		          5
#define MRCC_CFGR_HPRE2   		          6
#define MRCC_CFGR_HPRE3   		          7
#define MRCC_CFGR_PPRE10  		          8  
#define MRCC_CFGR_PPRE11  		          9      
#define MRCC_CFGR_PPRE12  		          10
#define MRCC_CFGR_PPRE20  		          11
#define MRCC_CFGR_PPRE21  		          12
#define MRCC_CFGR_PPRE22  		          13
#define MRCC_CFGR_ADCPRE0 		          14
#define MRCC_CFGR_ADCPRE1 		          15
#define MRCC_CFGR_PLLSRC 		          16
#define MRCC_CFGR_PLLXTPRE 		          17
#define MRCC_CFGR_PLLMUL0 		          18
#define MRCC_CFGR_PLLMUL1 		          19
#define MRCC_CFGR_PLLMUL2 		          20
#define MRCC_CFGR_PLLMUL3 		          21
#define MRCC_CFGR_USBPRE   		          22
#define MRCC_CFGR_MCO0   		          24
#define MRCC_CFGR_MCO1   		          25
#define MRCC_CFGR_MCO2   		          26

/*RCC Clock Interrupt Register Bit Definitions*/
/*To Do*/

/*RCC AHB1 peripheral reset register Bit Definitions*/
/*To Do*/

/*RCC AHB2 peripheral reset register Bit Definitions*/
/*To Do*/

/*RCC APB1 peripheral reset register Bit Definitions*/
/*To Do*/

/*RCC APB2 peripheral reset register Bit Definitions*/
/*To Do*/

/*RCC AHB Peripheral Clock Enable Register Bit Definitions*/
#define MRCC_AHBENR_DMA1EN               0
#define MRCC_AHBENR_DMA2EN			     1
#define MRCC_AHBENR_SRAMEN               2
#define MRCC_AHBENR_FLITFEN              4
#define MRCC_AHBENR_CRCEN                6
#define MRCC_AHBENR_FSMCEN               8
#define MRCC_AHBENR_SDIOEN               10

/*RCC APB1 Peripheral Clock Enable Register Bit Definitions*/
#define MRCC_APB1ENR_TIM2EN               0
#define MRCC_APB1ENR_TIM3EN		          1
#define MRCC_APB1ENR_TIM4EN		          2
#define MRCC_APB1ENR_TIM5EN		          3
#define MRCC_APB1ENR_TIM6EN		          4
#define MRCC_APB1ENR_TIM7EN		          5
#define MRCC_APB1ENR_TIM12EN	          6
#define MRCC_APB1ENR_TIM13EN	          7
#define MRCC_APB1ENR_TIM14EN	          8
#define MRCC_APB1ENR_WWDGEN               11
#define MRCC_APB1ENR_SPI2EN		          14
#define MRCC_APB1ENR_SPI3EN               15
#define MRCC_APB1ENR_USART2EN             17
#define MRCC_APB1ENR_USART3EN             18
#define MRCC_APB1ENR_UART4EN              19
#define MRCC_APB1ENR_UART5EN              20
#define MRCC_APB1ENR_I2C1EN               21
#define MRCC_APB1ENR_I2C2EN               22
#define MRCC_APB1ENR_USBEN                23
#define MRCC_APB1ENR_CANEN                25
#define MRCC_APB1ENR_BKPEN                27
#define MRCC_APB1ENR_PWREN                28
#define MRCC_APB1ENR_DACEN                29

/*RCC APB2 Peripheral Clock Enable Register Bit Definitions*/
#define MRCC_APB2ENR_AFIOEN               0
#define MRCC_APB2ENR_GPIOAEN	          2
#define MRCC_APB2ENR_GPIOBEN	          3
#define MRCC_APB2ENR_GPIOCEN	          4
#define MRCC_APB2ENR_ADC1EN		          9
#define MRCC_APB2ENR_ADC2EN		          10
#define MRCC_APB2ENR_TIM1EN		          11
#define MRCC_APB2ENR_SPI1EN               12
#define MRCC_APB2ENR_TIM8EN               13
#define MRCC_APB2ENR_USART1EN             14
#define MRCC_APB2ENR_ADC3EN               15
#define MRCC_APB2ENR_TIM9EN               19
#define MRCC_APB2ENR_TIM10EN              20
#define MRCC_APB2ENR_TIM11EN              21

/*RCC AHB1 peripheral clock enable in low power mode register Bit Definitions*/
/*To Do*/

/*RCC AHB2 peripheral clock enable in low power mode register Bit Definitions*/
/*To Do*/

/*RCC APB1 peripheral clock enable in low power mode register Bit Definitions*/
/*To Do*/

/*RCC APB2 peripheral clock enable in low power mode register Bit Definitions*/
/*To Do*/

/*RCC Backup domain control register Bit Definitions*/
#define MRCC_BDCR_LSEON                   0
#define MRCC_BDCR_LSERDY                  1
#define MRCC_BDCR_LSEBYP                  2
#define MRCC_BDCR_RTCSEL0                 8
#define MRCC_BDCR_RTCSEL1                 9
#define MRCC_BDCR_RTCEN                   15
#define MRCC_BDCR_BDRST                   16

/*RCC clock control & status register Bit Definitions*/
#define MRCC_CSR_LSION                    0
#define MRCC_CSR_LSIRDY	                  1
#define MRCC_CSR_RMVF	                  24
#define MRCC_CSR_PINRSTF                  26
#define MRCC_CSR_PORRSTF                  27
#define MRCC_CSR_SFTRSTF                  28
#define MRCC_CSR_IWDGRSTF                 29
#define MRCC_CSR_WWDGRSTF                 30
#define MRCC_CSR_LPWRRSTF                 31

/*RCC spread spectrum clock generation register Bit Definitions*/
/*To Do*/



/*RCC Clock Configuration Register Masks*/
#define MRCC_SW_MASK       0xFFFFFFFC /*(1111 1111) (1111 1111) (1111 1111) (1111 1100)*/
#define MRCC_SWS_MASK      0x0000000C
#define MRCC_CFGR_PLL_MASK 0xFF00FFFF

/*MAX Values for PLL config parameters*/
#define MRCC_PLL_MIN_MUL_FACT       	 0
#define MRCC_PLL_MAX_MUL_FACT       	 15

#define MRCC_PLL_MIN_USBCLOCK_PRESCALER  0
#define MRCC_PLL_MAX_USBCLOCK_PRESCALER  1

/*RCC Bus Clock Configuration Register Masks*/
#define MRCC_HPRE_MASK  0xFFFFFF0F
#define MRCC_PPRE1_MASK 0xFFFFF8FF /*(1111 1111) (1111 1111) (1111 1000) (1111 1111)*/
#define MRCC_PPRE2_MASK 0xFFFFC7FF /*(1111 1111) (1111 1111) (1100 0111) (1111 1111)*/

/*RCC Peripheral IDs and Config Defines*/
#define MRCC_MAX_PERIPHERALS              93
#define MRCC_PERIPHERAL_NUM_MASK          0x0000001F
#define MRCC_PERIPHERAL_PORT_SHIFT_FACTOR 5  


#endif
