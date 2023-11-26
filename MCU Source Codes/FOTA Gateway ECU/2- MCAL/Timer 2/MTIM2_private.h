/**********************************/
/* Author    : Amir Galal         */
/* SWC       : TIMER2             */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : October 20,2023    */
/* Last Edit : N/A                */
/**********************************/
#ifndef _MTIM2_PRIVATE_H_
#define _MTIM2_PRIVATE_H_

#define MTIM2_BASE_ADDRESS ((volatile u32*)0x40000000)

typedef struct 
{
	volatile u32 CR1	;
	volatile u32 CR2	;
	volatile u32 SMCR	;
	volatile u32 DIER	;
	volatile u32 SR		;
	volatile u32 EGR 	;
	volatile u32 CCMR1	;
	volatile u32 CCMR2	;
	volatile u32 CCER 	;
	volatile u32 CNT	;
	volatile u32 PSC	;
	volatile u32 ARR	;
	volatile u32 reserved;
	volatile u32 CCR1	;
	volatile u32 CCR2	;
	volatile u32 CCR3	;
	volatile u32 CCR4	;
	volatile u32 reserved1;
	volatile u32 DCR	;
	volatile u32 DMAR	;
	
}TIM2_t;

#define MTIM2 ((volatile TIM2_t*)0x40000000)

#endif
