#ifndef NVIC_INTERFACE_H_
#define NVIC_INTERFACE_H_

#include <stdint.h>

typedef enum
{
	WWDG_IRQ,							/*Window Watchdog interrupt*/
	PVD_IRQ,                            /*PVD through EXTI line detection interrupt*/
	TAMP_IRQ,                           /*Tamper and TimeStamp interrupts through the EXTI line*/
	RTC_IRQ,                            /*RTC Wake up interrupt through the EXTI line*/
	FLASH_IRQ,                          /*Flash global interrupt*/
	RCC_IRQ,                            /*RCC global interrupt*/
	EXTI0_IRQ,                          /*EXTI Line0 interrupt*/
	EXTI1_IRQ,                          /*EXTI Line1 interrupt*/
	EXTI2_IRQ,                          /*EXTI Line2 interrupt*/
	EXTI3_IRQ,                          /*EXTI Line3 interrupt*/
	EXTI4_IRQ,                          /*EXTI Line4 interrupt*/
	DMA1_channel1_IRQ,                  /*DMA1 channel1 global interrupt*/
	DMA1_channel2_IRQ,                  /*DMA1 channel2 global interrupt*/
	DMA1_channel3_IRQ,                  /*DMA1 channel3 global interrupt*/
	DMA1_channel4_IRQ,                  /*DMA1 channel4 global interrupt*/
	DMA1_channel5_IRQ,                  /*DMA1 channel5 global interrupt*/
	DMA1_channel6_IRQ,                  /*DMA1 channel6 global interrupt*/
	DMA1_channel7_IRQ,                  /*DMA1 channel7 global interrupt*/
	ADC1_2_IRQ,                            /*ADC1, ADC2 and ADC3 global interrupts*/
	CAN1_TX_IRQ,                        /*CAN1 TX interrupts*/
	CAN1_RX0_IRQ,                       /*CAN1 RX0 interrupts*/
	CAN1_RX1_IRQ,                       /*CAN1 RX1 interrupts*/
	CAN1_SCE_IRQ,                       /*CAN1 SCE interrupt*/
	EXTI9_5_IRQ,                        /*EXTI Line[9:5] interrupts*/
	TIM1_BRK_IRQ,                  /*TIM1 break interrupt and TIM9 global interrupt*/
	TIM1_UP_IRQ,                  /*TIM1 Update interrupt and TIM10 global interrupt*/
	TIM1_TRG_COM_IRQ,             /*TIM1 Trigger and Commutation interrupts and TIM11 global interrupt*/
	TIM1_CC_IRQ,                        /*TIM1 Capture compare interrupt*/
	TIM2_IRQ,                           /*TIM2 global interrupt*/
	TIM3_IRQ,                           /*TIM3 global interrupt*/
	TIM4_IRQ,                           /*TIM4 global interrupt*/
	I2C1_EV_IRQ,                        /*I2C1 error interrupt*/
	I2C1_ER_IRQ,                        /*I2C1 error interrupt*/
	I2C2_EV_IRQ,                        /*I2C2 error interrupt*/
	I2C2_ER_IRQ,                        /*I2C2 error interrupt*/
	SPI1_IRQ,                           /*SPI1 global interrupt*/
	SPI2_IRQ,                           /*SPI2 global interrupt*/
	USART1_IRQ,                         /*USART1 global interrupt*/
	USART2_IRQ,                         /*USART2 global interrupt*/
	USART3_IRQ,                         /*USART3 global interrupt*/
	EXTI15_10_IRQ,                      /*EXTI Line[15:10] interrupts*/
	RTC_Alarm_IRQ,                      /*RTC Alarms (A and B) through EXTI line interrupt*/
	OTG_FS_WKUP_IRQ,                    /*USB On-The-Go FS Wake up through EXTI line interrupt*/
	TIM5_IRQ,                           /*TIM5 global interrupt*/
	SPI3_IRQ,                           /*SPI3 global interrupt*/
	UART4_IRQ,                          /*USART4 global interrupt*/
	UART5_IRQ,                          /*USART5 global interrupt*/
	TIM6_IRQ,                       /*TIM6 global interrupt, DAC1 and DAC2 under run error interrupts*/
	TIM7_IRQ,                           /*TIM7 global interrupt*/
	DMA2_channel1_IRQ,                   /*DMA2 channel1 global interrupt*/
	DMA2_channel2_IRQ,                   /*DMA2 channel2 global interrupt*/
	DMA2_channel3_IRQ,                   /*DMA2 channel3 global interrupt*/
	DMA2_channel4_IRQ,                   /*DMA2 channel4 global interrupt*/
	DMA2_channel5_IRQ,                   /*DMA2 channel5 global interrupt*/
	ETH_IRQ,
	ETH_WKUP_IRQ,
	CAN2_TX_IRQ,                        /*CAN2 TX interrupts*/
	CAN2_RX0_IRQ,                       /*CAN2 RX0 interrupts*/
	CAN2_RX1_IRQ,                       /*CAN2 RX1 interrupts*/
	CAN2_SCE_IRQ,                       /*CAN2 SCE interrupt*/
	OTG_FS_IRQ,                         /*USB On The Go FS global interrupt*/
}IRQ_Num;

typedef enum
{
	BitsGroup4_Sub0 =3 ,
	BitsGroup3_Sub1,
	BitsGroup2_Sub2,
	BitsGroup1_Sub3,
	BitsGroup0_Sub4
}PRI_Group_t;

typedef enum
{
	IRQ_Priority0 ,
	IRQ_Priority1 ,
	IRQ_Priority2 ,
	IRQ_Priority3 ,
	IRQ_Priority4 ,
	IRQ_Priority5 ,
	IRQ_Priority6 ,
	IRQ_Priority7 ,
	IRQ_Priority8 ,
	IRQ_Priority9 ,
	IRQ_Priority10 ,
	IRQ_Priority11 ,
	IRQ_Priority12 ,
	IRQ_Priority13 ,
	IRQ_Priority14 ,
	IRQ_Priority15
}IRQ_Priority_t;



uint8_t NVIC_u8EnableINT(uint8_t IRQ_u8Num);

uint8_t NVIC_u8DisableINT(uint8_t IRQ_u8Num);

uint8_t NVIC_u8SetPendingFlag(uint8_t IRQ_u8Num);

uint8_t NVIC_u8ClearPendingFlag(uint8_t IRQ_u8Num);

uint8_t NVIC_u8GetActiveFlag(uint8_t IRQ_u8Num , uint8_t * ActiveFlag_u8val);

uint8_t SCB_u8SetPriorityGrouping(uint8_t copy_u8priority_group);

uint8_t NVIC_u8SetIRQPriority(uint8_t IRQ_u8Num , uint8_t IRQ_u8priority);



#endif /* NVIC_INTERFACE_H_ */
