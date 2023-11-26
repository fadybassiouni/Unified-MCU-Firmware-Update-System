#ifndef EXTI_INTERFACE_H_
#define EXTI_INTERFACE_H_



/******************* Procedure for using external pins IRQ ********************/
            /* Enable GPIO and AFIO clocks through RCC controller */
             /** Config the required GPIO pin as floating input **/
   /*** Map the external required pins to the lines through AFIO_EXTICRx ***/
      /**** Config the required line through EXTI_u8ConfigLine() fn ****/
              /***** Enable NVIC IRQn for the required line *****/
               /****** Set the required call back function *****/
           /******* Enable Interrupt mask in EXTI controller *******/
/******************************************************************************/
/*Triggering mode options*/
#define RISING_EDGE_TRIGGERING    		1
#define FALLING_EDGE_TRIGGERING   		3
#define ON_CHANGE_TRIGGERING      		5


//#define EXTI9_5_LINES_IRQ 				1
/* Please comment the def line if it isn't needed*/
#define EXTI0_LINE              		0
//#define EXTI1_LINE            		1
//#define EXTI2_LINE            		2
//#define EXTI3_LINE            		3
#define EXTI4_LINE            		4
		
//#define EXTI5_LINE            		5
//#define EXTI6_LINE            		6
#define EXTI7_LINE            			7
//#define EXTI8_LINE            		8
//#define EXTI9_LINE            		9
		
//#define EXTI10_LINE           		10
//#define EXTI11_LINE           		11
//#define EXTI12_LINE           		12
//#define EXTI13_LINE           		13
//#define EXTI14_LINE           		14
//#define EXTI15_LINE           		15

//#define EXTI9_5_LINES_IRQ					EXTI7_LINE


/** NOTE: lines from 5-9 and from 10-15 each have only one IRQn Handler **/

uint8_t EXTI_voidSetCallBackFn     (uint8_t Copy_u8EXTI_Line , void (*pvoidfCallback)(void));
uint8_t EXTI_u8ConfigLine       (uint8_t Copy_u8EXTI_Line , uint8_t Copy_u8TriggeringMode);

uint8_t EXTI_u8IntDisable(uint8_t Copy_EXTI_Num);
uint8_t EXTI_u8SetTrigger(uint8_t Copy_EXTI_Num ,uint8_t Copy_TriggerState );
uint8_t EXTI_u8ClearPendingFlag(uint8_t Copy_EXTI_Num);
uint8_t EXTI_u8GetPendingFlag(uint8_t Copy_EXTI_Num , uint8_t *Copy_PenValue);
uint8_t EXTI_u8IntEnable(uint8_t Copy_EXTI_Num);


#endif /* EXTI_INTERFACE_H_ */
