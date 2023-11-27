/**********************************/
/* Author    : Muhammad Reda      */
/* SWC       : Timer 	          */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : October 23,2023    */
/* Last Edit : N/A                */
/**********************************/

#ifndef MTIM1_PRIVATE_H
#define MTIM1_PRIVATE_H

// 0x4001 0000
#define TIM1_CR1	*((volatile u32*)0x40012C00)
#define TIM1_CR2	*((volatile u32*)0x40012C04)
#define TIM1_CCMR1	*((volatile u32*)0x40012C18)
#define	TIM1_CCER	*((volatile u32*)0x40012C20)
#define	TIM1_PSC	*((volatile u32*)0x40012C28)
#define	TIM1_CCR1	*((volatile u32*)0x40012C34)
#define TIM1_ARR 	*((volatile u32*)0x40012C2C)
#define TIM1_BDTR	*((volatile u32*)0x40012C44)


#endif
