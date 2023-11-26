/**********************************/
/* Author    : Amir Galal         */
/* SWC       : TIMER2             */
/* Layer     : MCAL  		      */
/* Version   : 1.0                */
/* Date      : October 20,2023    */
/* Last Edit : N/A                */
/**********************************/
#ifndef _MTIM2_INTERFACE_H_
#define _MTIM2_INTERFACE_H_

#define MTIM2_AUTORELOAD_VALUE 49999
#define MTIM2_UNDERFLOW_TIME   (MTIM2_AUTORELOAD_VALUE+1)

STD_error_t MTIM2_stderrStartPeriodic(u32 ARG_u32Ticks, void (*ARG_pvoidfuncUserFunction)(void));

void MTIM2_voidInit(void);
void MTIM2_voidEnableTimer(void);
void MTIM2_voidDisableTimer(void);

void MTIM2_voidEnableInterrupt(void);
void MTIM2_voidDisableInterrupt(void);


#endif
