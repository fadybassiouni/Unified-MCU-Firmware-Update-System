/**********************************/
/* Author    : Fady Bassiouni     */
/* SWC       : SysTick            */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : September 18,2023  */
/* Last Edit : N/A                */
/**********************************/
#ifndef _STK_PRIVATE_H
#define _STK_PRIVATE_H

#define MSTK_BASE_ADDRESS ((volatile u32*)0xE000E010)

typedef struct 
{
	volatile u32 CTRL;
	volatile u32 LOAD;
	volatile u32 VAL;
	volatile u32 CALIB;
}MSTK_t;

#define MSTK ((volatile MSTK_t *)MSTK_BASE_ADDRESS)

/*SysTick Control Register Bits*/
#define MSTK_CTRL_ENABLE     0
#define MSTK_CTRL_TICKINT    1
#define MSTK_CTRL_CLKSOURCE  2
#define MSTK_CTRL_COUNTFLAG  16

#endif
